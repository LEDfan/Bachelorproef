#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>
#include <set>
#include <unordered_map>

class LogicBridge : public QObject
{
        Q_OBJECT

public:
signals:
        void SetGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid);
        void updateMarkers();
};
