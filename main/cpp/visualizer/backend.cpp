#include "backend.h"
#include <QtCore/QVariant>
#include <iostream>

BackEnd::BackEnd(QObject* parent) : QObject(parent) {}

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
        QVariant returnVal;
        std::cout << _map << std::endl;
        QMetaObject::invokeMethod(_map, "addMarker", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal),
                                  Q_ARG(QVariant, 51.2), Q_ARG(QVariant, 4.4), Q_ARG(QVariant, "test"));
}

void BackEnd::onMarkerClicked(const QString& idOfClicked)
{
        std::cout << "CPP: Marker clicked " << idOfClicked.toStdString() << std::endl;
}

void BackEnd::setObjects(QObject* map) { _map = map; }
