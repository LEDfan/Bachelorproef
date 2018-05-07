#pragma once

#include <QtCore/QObject>
#include <gengeopop/GeoGrid.h>
#include <thread>

class GuiController
{
public:
        GuiController();
        GuiController(const GuiController& b) = delete;
        GuiController& operator=(const GuiController& b) = delete;

        /**
         * Block until the visualizer is closed.
         */
        void Join();

private:
        std::unique_ptr<std::thread> m_thread;          ///< The thread that the visualizer QT application is run on
};
