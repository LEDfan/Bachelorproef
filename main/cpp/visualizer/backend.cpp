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

        // Place the new markers
        for (const std::shared_ptr<gengeopop::Location>& loc : *m_grid) {
                PlaceMarker(loc->getCoordinate(), std::to_string(loc->getID()), loc->getPopulation());
        }
}

void Backend::OnMarkerClicked(int idOfClicked)
{
        auto loc = m_grid->GetById(idOfClicked);
        clearSelection();
        addToSelectionIfNoDuplicate(loc);
        emitLocations();
}

void Backend::SetObjects(QObject* map)
{
        m_map = map;

        PlaceMarkers();
}

void Backend::PlaceMarker(Coordinate coordinate, std::string id, unsigned int population)
{
        QVariant returnVal;
        QMetaObject::invokeMethod(m_map, "addMarker", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal),
                                  Q_ARG(QVariant, coordinate.latitude), Q_ARG(QVariant, coordinate.longitude),
                                  Q_ARG(QVariant, QString(id.c_str())),
                                  Q_ARG(QVariant, std::min(50.0, 10 + population * 0.0015)));
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
onPressed : {
}
        m_selection.clear();
        emitLocations();
}

void Backend::emitLocations() { emit LocationsSelected(m_selection); }

void Backend::OnExtraMarkerClicked(int idOfClicked)
{
        auto loc = m_grid->GetById(idOfClicked);
        addToSelectionIfNoDuplicate(loc);
        emitLocations();
}

void Backend::addToSelectionIfNoDuplicate(std::shared_ptr<gengeopop::Location> loc)
{
        for (auto locInLoop : m_selection) {
                if (locInLoop->getID() == loc->getID()) {
                        return;
                }
        }
        m_selection.push_back(loc);
}

void Backend::selectArea(double slat, double slong, double elat, double elong)
{
        std::cout << "START: " << slat << "," << slong << std::endl;
        std::cout << "END: " << elat << "," << elong << std::endl;
}
