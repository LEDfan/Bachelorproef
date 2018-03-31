#include "backend.h"
#include <QtCore/QFileInfo>
#include <QtCore/QPointF>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/qdebug.h>
#include <QtQml/QQmlProperty>
#include <cmath>
#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/io/GeoGridJSONReader.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <iostream>
#include <util/Stopwatch.h>

Backend::Backend(QObject* parent)
    : QObject(parent), m_grid(std::make_shared<gengeopop::GeoGrid>()), m_markers(), m_selection(), m_unselection()
{
}

Backend::Backend(const Backend&)
    : QObject(), m_grid(std::make_shared<gengeopop::GeoGrid>()), m_markers(), m_selection(), m_unselection()
{
}

Backend& Backend::operator=(const Backend& b)
{
        m_grid      = b.m_grid;
        m_selection = b.m_selection;
        return *this;
}

void Backend::LoadGeoGridFromFile(const QString& file, QObject* errorDialog)
{
        QUrl                         info(file);
        std::ifstream                inputFile(info.toLocalFile().toStdString());
        gengeopop::GeoGridJSONReader reader;
        try {
                m_grid = reader.read(inputFile);
                m_grid->finalize();
        } catch (const std::exception& e) {
                QMetaObject::invokeMethod(errorDialog, "open");
                QQmlProperty(errorDialog, "text").write(QString("Error: ") + e.what());
        }
        PlaceMarkers();
}

void Backend::PlaceMarkers()
{
        // Clear the present markers
        QMetaObject::invokeMethod(m_map, "clearMap");

        // Place the commutes of the selection

        for (auto loc : m_selection) {
                for (auto commute : loc->getIncomingCommuningCities()) {
                        auto otherCity = commute.first;
                        addCommuteLine(otherCity->getCoordinate(), loc->getCoordinate(), commute.second);
                }
        }

        // Place the new markers
        for (const std::shared_ptr<gengeopop::Location>& loc : *m_grid) {
                bool selected = m_selection.find(loc) != m_selection.end();
                PlaceMarker(loc->getCoordinate(), std::to_string(loc->getID()), loc->getPopulation(), selected);
        }
}

void Backend::OnMarkerClicked(int idOfClicked)
{
        auto loc = m_grid->GetById(idOfClicked);

        ClearSelection();
        toggleSelectionOfLocation(loc);

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::SetObjects(QObject* map)
{
        m_map = map;

        PlaceMarkers();
}

void Backend::PlaceMarker(Coordinate coordinate, std::string id, unsigned int population, bool selected)
{
        QVariant returnVal;
        QMetaObject::invokeMethod(m_map, "addMarker", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal),
                                  Q_ARG(QVariant, coordinate.latitude), Q_ARG(QVariant, coordinate.longitude),
                                  Q_ARG(QVariant, QString(id.c_str())),
                                  Q_ARG(QVariant, std::min(50.0, 10 + population * 0.0015)), Q_ARG(QVariant, selected));
        m_markers[id] = qvariant_cast<QObject*>(returnVal);
}

void Backend::SaveGeoGridToFile(const QString& fileLoc, QObject* errorDialog)
{
        QUrl                         info(fileLoc);
        std::ofstream                outputFile(info.toLocalFile().toStdString());
        gengeopop::GeoGridJSONWriter writer;
        try {
                writer.write(m_grid, outputFile);
        } catch (const std::exception& e) {
                QMetaObject::invokeMethod(errorDialog, "open");
                QQmlProperty(errorDialog, "text").write(QString("Error: ") + e.what());
        }
        outputFile.close();
}

void Backend::ClearSelection()
{
        m_unselection.clear();
        m_unselection.insert(m_selection.begin(), m_selection.end());
        m_selection.clear();
}

void Backend::ClearSelectionAndRender()
{
        for (const std::shared_ptr<gengeopop::Location>& loc : m_selection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "red");
        }
        for (const std::shared_ptr<gengeopop::Location>& loc : m_unselection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "red");
        }
        m_selection.clear();
        m_unselection.clear();

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::emitLocations() { emit LocationsSelected(m_selection); }

void Backend::OnExtraMarkerClicked(int idOfClicked)
{
        auto loc = m_grid->GetById(idOfClicked);
        toggleSelectionOfLocation(loc);

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::toggleSelectionOfLocation(std::shared_ptr<gengeopop::Location> loc)
{
        if (m_selection.find(loc) == m_selection.end()) {
                m_selection.insert(loc);
                m_unselection.erase(loc);
        } else {
                m_selection.erase(loc);
                m_unselection.insert(loc);
        }
}

void Backend::selectArea(double slat, double slong, double elat, double elong)
{
        try {
                m_unselection.clear();
                std::set<std::shared_ptr<gengeopop::Location>> previousSelection = m_selection;
                m_selection = m_grid->inBox(slong, slat, elong, elat);
                std::set_difference(previousSelection.begin(), previousSelection.end(), m_selection.begin(),
                                    m_selection.end(), std::inserter(m_unselection, m_unselection.end()));

        } catch (std::exception& e) {
                // Can happen when geogrid is not yet loaded
                std::cout << e.what() << std::endl;
                return;
        }

        emitLocations();
        UpdateColorOfMarkers();
}

void Backend::UpdateColorOfMarkers()
{
        for (const std::shared_ptr<gengeopop::Location>& loc : m_unselection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "red");
        }
        m_unselection.clear();
        for (const std::shared_ptr<gengeopop::Location>& loc : m_selection) {
                auto* marker = m_markers[std::to_string(loc->getID())]->findChild<QObject*>("rect");
                marker->setProperty("color", "blue");
        }
}

void Backend::addCommuteLine(Coordinate from, Coordinate to, double amount)
{
        QVariant retVal;
        QMetaObject::invokeMethod(m_map, "addCommute", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal),
                                  Q_ARG(QVariant, from.latitude), Q_ARG(QVariant, from.longitude),
                                  Q_ARG(QVariant, to.latitude), Q_ARG(QVariant, to.longitude));
}

void Backend::selectAll()
{
        for (auto it = m_grid->begin(); it != m_grid->end(); it++) {
                m_selection.insert(*it);
        }

        emitLocations();
        PlaceMarkers();
}
