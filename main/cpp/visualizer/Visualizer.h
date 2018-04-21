#pragma once

#include <gengeopop/GeoGrid.h>
#include <QtCore/QObject>
#include <thread>

class Visualizer {
public:
    Visualizer();

    void setGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid);

    void forceUpdateMarkers();

private:
    QObject* m_rootContext = nullptr;
    std::unique_ptr<std::thread> m_thread;

};


