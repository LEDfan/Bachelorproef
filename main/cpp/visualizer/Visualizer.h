#pragma once

#include <QtCore/QObject>
#include <gengeopop/GeoGrid.h>
#include <thread>
#include <visualizer/backends/LogicBridge.h>

class Visualizer
{
public:
        Visualizer();

        void setGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid);

        void forceUpdateMarkers();

        void join();

private:
        QObject*                     m_rootContext = nullptr;
        std::unique_ptr<std::thread> m_thread;
        LogicBridge*                 m_bridge = nullptr;
};
