/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#pragma once

#include <QApplication>
#include <QtCore/QObject>
#include <QtQml/QQmlApplicationEngine>
#include <QtWidgets/QApplication>
#include <memory>
#include <thread>

#include "GuiControllerBackend.h"
#include <gengeopop/GeoGrid.h>
#include <sim/BaseController.h>

namespace stride {

/**
 * A controller for the simulation that uses Qt to give the user the ability of controlling the steps.
 */
class GuiController : public BaseController
{
public:
        /// Construct the controller with a ptree containing the configuration, this contains the information needed to
        /// construct the BaseController
        explicit GuiController(const boost::property_tree::ptree& configPt);
        GuiController(const GuiController& b) = delete;
        GuiController& operator=(const GuiController& b) = delete;

        ~GuiController();

        /// Start the Gui
        void Control() override;

        /// Returns the QQmlApplicationEngine used to construct the Gui, so other Gui components can register themselves
        /// on the same engine.
        QQmlApplicationEngine* GetEngine();

private:
        std::shared_ptr<int>                   m_argc;   ///< Used for initializing m_app, is always 0.
        std::unique_ptr<QQmlApplicationEngine> m_engine; ///< The engine used to register QML files
        GuiControllerBackend* m_backend; ///< The backend used in the QML to control the behaviour of the simulation
};
} // namespace stride
