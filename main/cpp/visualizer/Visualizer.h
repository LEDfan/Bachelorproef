#pragma once

#include <QtCore/QObject>
#include <gengeopop/GeoGrid.h>
#include <thread>

class Visualizer
{
public:
        Visualizer();
        Visualizer(const Visualizer& b) = delete;
        Visualizer& operator=(const Visualizer& b) = delete;

        /**
         * Shows the given geogrid in the visualizer. Places the markers.
         * @param grid The grid we want to show
         */
        void SetGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid);

        void ForceUpdateMarkers();

        /**
         * Block until the visualizer is closed.
         */
        void Join();

private:
        QObject*                     m_rootContext = nullptr; ///< The root context of the main QML file
        std::unique_ptr<std::thread> m_thread;          ///< The thread that the visualizer QT application is run on
        bool                         m_setGrid = false; ///< If the grid was set for the visualizer
};
