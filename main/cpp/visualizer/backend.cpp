#include "backend.h"
#include <QtCore/QVariant>
#include <iostream>

BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
        _grid = std::make_shared<gengeopop::GeoGrid>();
        std::shared_ptr<gengeopop::Location> newLoc =
            std::make_shared<gengeopop::Location>(0, 0, 0, Coordinate(0, 0, 51.2, 4.4));
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
        std::cout << "Placing markers" << std::endl;
        std::cout << _map << std::endl;
        placeMarker({0, 0, 51.1, 4.4}, "1");
}

void BackEnd::onMarkerClicked(const QString& idOfClicked)
{
        std::cout << "CPP: Marker clicked " << idOfClicked.toStdString() << std::endl;
}

void BackEnd::setObjects(QObject* map) { _map = map; }

void BackEnd::placeMarker(Coordinate coordinate, std::string id)
{
        QVariant returnVal;
        QMetaObject::invokeMethod(_map, "addMarker", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal),
                                  Q_ARG(QVariant, coordinate.latitude), Q_ARG(QVariant, coordinate.longitude),
                                  Q_ARG(QVariant, "test"));
}
