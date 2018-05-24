#pragma once

#include "Launcher.h"

#include <thread>

class GuiLauncher
{
public:
        /// Starts launcher and blocks until the launcher is closed,
        static void Start();
};
