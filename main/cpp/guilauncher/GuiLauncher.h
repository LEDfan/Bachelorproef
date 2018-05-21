#pragma once

#include "Launcher.h"

#include <thread>

class GuiLauncher
{
public:
        GuiLauncher();
        GuiLauncher(const GuiLauncher& b) = delete;
        GuiLauncher& operator=(const GuiLauncher& b) = delete;

        /// Starts launcher and blocks until the launcher is closed,
        void Start();

private:
        std::unique_ptr<std::thread> m_thread;   ///< The thread that the mapviewer application is run on
        Launcher                     m_launcher; ///< THe launcher that we configure in the GUI and then call to launch
};
