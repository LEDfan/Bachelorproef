#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>

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
        void onMarkerClicked(const QString& idOfClicked);

signals:
        void userNameChanged();

private:
        QString                             m_userName;
        QObject*                            _map = nullptr;
        std::shared_ptr<gengeopop::GeoGrid> _grid;

        void placeMarker(Coordinate coordinate, std::string id);

        /*
         * Places the markers on the map, according to the current checked boxes.
         * @Pre: _map is initialized correctly and holds the map we want to place markers on
         */
        void placeMarkers();
};

#endif // BACKEND_H
