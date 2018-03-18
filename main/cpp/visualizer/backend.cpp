#include "backend.h"
#include <QtCore/QVariant>
#include <cmath>
#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <iostream>

Backend::Backend(QObject* parent) : QObject(parent)
{
        // This is to be removed once the reader is working
        m_grid = std::make_shared<gengeopop::GeoGrid>();
        std::shared_ptr<gengeopop::Location> newLoc =
            std::make_shared<gengeopop::Location>(0, 0, 0, Coordinate(0, 0, 51.2, 4.4), "NAAM");
        auto newCC = std::make_shared<gengeopop::School>();
        newLoc->addContactCenter(newCC);
        m_grid->addLocation(newLoc);

        std::shared_ptr<gengeopop::Location> newLoc1 =
            std::make_shared<gengeopop::Location>(1, 1, 1000000, Coordinate(0, 0, 51.201, 4.4), "Other");
        auto newCC1 = std::make_shared<gengeopop::Workplace>();
        auto aPool  = std::make_shared<gengeopop::ContactPool>();
        newCC1->addPool(aPool);
        newLoc1->addContactCenter(newCC1);
        m_grid->addLocation(newLoc1);
}

void Backend::LoadGeoGridFromFile(const QString& file)
{
        // TODO Send the file to the geoGridReader and keep the geoGrid Loaded
        std::cout << file.toStdString() << std::endl;
        PlaceMarkers();
}

void Backend::PlaceMarkers()
{
        // Clear the present markers
        QMetaObject::invokeMethod(m_map, "clearMapItems");

        // Place the new markers
        for (std::shared_ptr<gengeopop::Location> loc : m_grid->topK(100)) {
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
                                  Q_ARG(QVariant, 15.0 + std::max(0.0, 2 * std::log2(population))));
}

void Backend::SaveGeoGridToFile(const QString& fileLoc)
{
        gengeopop::GeoGridJSONWriter writer;
        std::ofstream                outputFile(fileLoc.toStdString());
        writer.write(m_grid, outputFile);
        outputFile.close();
}

void Backend::clearSelection()
{
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
