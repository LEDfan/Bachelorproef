#pragma once

#include "Launcher.h"
#include <thread>

class GuiLauncher
{
public:
        GuiLauncher();
        GuiLauncher(const GuiLauncher& b) = delete;
        GuiLauncher& operator=(const GuiLauncher& b) = delete;

        /**
         * Block until the launcher is closed.
         */
        void StartSimulation();

private:
        std::unique_ptr<std::thread> m_thread;   ///< The thread that the visualizer QT application is run on
        Launcher                     m_launcher; // THe launcher that we configure in the GUI and then call to launch
};
