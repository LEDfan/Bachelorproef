#include "backend.h"
#include <QtCore/QVariant>
#include <cmath>
#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <iostream>

BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
        _parent = parent;
        _grid   = std::make_shared<gengeopop::GeoGrid>();
        std::shared_ptr<gengeopop::Location> newLoc =
            std::make_shared<gengeopop::Location>(0, 0, 0, Coordinate(0, 0, 51.2, 4.4), "NAAM");
        auto newCC = std::make_shared<gengeopop::School>();
        newLoc->addContactCenter(newCC);
        _grid->addLocation(newLoc);

        std::shared_ptr<gengeopop::Location> newLoc1 =
            std::make_shared<gengeopop::Location>(1, 1, 1000000, Coordinate(0, 0, 51.201, 4.4), "Other");
        auto newCC1 = std::make_shared<gengeopop::Workplace>();
        auto aPool  = std::make_shared<gengeopop::ContactPool>();
        newCC1->addPool(aPool);
        newLoc1->addContactCenter(newCC1);
        _grid->addLocation(newLoc1);
}

void BackEnd::loadGeoGridFromFile(const QString& file)
{
        // TODO Send the file to the geoGridReader and keep the geoGrid Loaded
        std::cout << file.toStdString() << std::endl;
        placeMarkers();
}

void BackEnd::placeMarkers()
{
        // Clear the present markers
        QMetaObject::invokeMethod(_map, "clearMapItems");

        // Place the new markers
        for (std::shared_ptr<gengeopop::Location> loc : _grid->topK(100)) {
                placeMarker(loc->getCoordinate(), std::to_string(loc->getID()), loc->getPopulation());
        }
}

void BackEnd::onMarkerClicked(int idOfClicked) { emit locationSelected(_grid->get(idOfClicked)); }

void BackEnd::setObjects(QObject* map)
{
        _map = map;

        placeMarkers();
}

void BackEnd::placeMarker(Coordinate coordinate, std::string id, unsigned int population)
{
        QVariant returnVal;
        QMetaObject::invokeMethod(_map, "addMarker", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal),
                                  Q_ARG(QVariant, coordinate.latitude), Q_ARG(QVariant, coordinate.longitude),
                                  Q_ARG(QVariant, QString(id.c_str())),
                                  Q_ARG(QVariant, 15.0 + std::max(0.0, 2 * std::log2(population))));
}

void BackEnd::saveGeoGridToFile(const QString& fileLoc)
{
        gengeopop::GeoGridJSONWriter writer;
        std::ofstream                outputFile(fileLoc.toStdString());
        writer.write(_grid, outputFile);
        outputFile.close();
}
