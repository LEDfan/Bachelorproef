#include "backend.h"
#include <iostream>
#include <QtCore/QVariant>

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
}

QString BackEnd::userName()
{
    return m_userName;
}

void BackEnd::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}


void BackEnd::loadGeoGridFromFile(const QString &file, QObject* map) {
    // TODO Send the file to the geoGridReader and keep the geoGrid Loaded
    std::cout << file.toStdString() << std::endl;
    _map = map;
    placeMarkers();
}

void BackEnd::placeMarkers() {
    // Clear the present markers
    QMetaObject::invokeMethod(_map, "clearMapItems");

    // Place the new markers
    std::cout << "Placing markers" << std::endl;
    QVariant returnVal;
    std::cout << _map << std::endl;
    QMetaObject::invokeMethod(_map,"addMarker", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal), Q_ARG(QVariant, 51.2), Q_ARG(QVariant, 4.4));
}
