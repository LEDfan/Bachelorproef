#include "backend.h"
#include <QtCore/QFileInfo>
#include <QtCore/QPointF>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtQml/QQmlProperty>
#include <cmath>
#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/io/GeoGridJSONReader.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <iostream>

Backend::Backend(QObject* parent) : QObject(parent), m_grid(std::make_shared<gengeopop::GeoGrid>()), m_selection() {}

Backend::Backend(const Backend&) : QObject(), m_grid(std::make_shared<gengeopop::GeoGrid>()), m_selection() {}

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
                std::cout << "Checking sel commutes" << loc->getIncomingCommuningCities().size() << std::endl;
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
        clearSelection();
        toggleSelectionOfLocation(loc);
        emitLocations();
        PlaceMarkers();
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

void Backend::clearSelection()
{
        m_selection.clear();
        emitLocations();
        PlaceMarkers();
}

void Backend::emitLocations() { emit LocationsSelected(m_selection); }

void Backend::OnExtraMarkerClicked(int idOfClicked)
{
        auto loc = m_grid->GetById(idOfClicked);
        toggleSelectionOfLocation(loc);
        emitLocations();
        PlaceMarkers();
}

void Backend::toggleSelectionOfLocation(std::shared_ptr<gengeopop::Location> loc)
{
        if (m_selection.find(loc) == m_selection.end()) {
                m_selection.insert(loc);
        } else {
                m_selection.erase(loc);
        }
}

void Backend::selectArea(double slat, double slong, double elat, double elong)
{
        std::vector<std::shared_ptr<gengeopop::Location>> selectedLocations;
        try {
                selectedLocations = m_grid->inBox(slong, slat, elong, elat);
        } catch (std::exception& e) {
                // Can happen when geogrid is not yet loaded
                std::cout << e.what() << std::endl;
                return;
        }

        m_selection.clear();
        for (auto location : selectedLocations) {
                m_selection.insert(location);
        }
        emitLocations();
        PlaceMarkers();
}

void Backend::addCommuteLine(Coordinate from, Coordinate to, double amount)
{
        QVariant retVal;
        std::cout << "ADding commute line" << std::endl;
        QMetaObject::invokeMethod(m_map, "addCommute", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retVal),
                                  Q_ARG(QVariant, from.longitude), Q_ARG(QVariant, from.latitude),
                                  Q_ARG(QVariant, to.longitude), Q_ARG(QVariant, to.latitude),
                                  Q_ARG(QVariant, std::max(1.0, std::log(amount))));
}
