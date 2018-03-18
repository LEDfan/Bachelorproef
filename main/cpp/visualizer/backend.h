#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>
#include <visualizer/objects/LocationQWrapper.h>

class BackEnd : public QObject
{
        Q_OBJECT

public:
        explicit BackEnd(QObject* parent = nullptr);

        Q_INVOKABLE
        void loadGeoGridFromFile(const QString& file);

        Q_INVOKABLE
        void setObjects(QObject* map);

        Q_INVOKABLE
        void onMarkerClicked(int idOfClicked);

        Q_INVOKABLE
        void saveGeoGridToFile(const QString& fileLoc);

signals:
        void locationSelected(std::shared_ptr<gengeopop::Location> location);

private:
        QString                             m_userName;
        QObject*                            _map    = nullptr;
        QObject*                            _parent = nullptr;
        std::shared_ptr<gengeopop::GeoGrid> _grid;

        void placeMarker(Coordinate coordinate, std::string id, unsigned int population = 400);

        /*
         * Places the markers on the map, according to the current checked boxes.
         * @Pre: _map is initialized correctly and holds the map we want to place markers on
         */
        void placeMarkers();
};
