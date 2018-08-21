#pragma once

#include <QtCore/QObject>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
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
        std::unique_ptr<QGuiApplication>       m_app;    ///< The main application that is used for the event loop.
        std::unique_ptr<QQmlApplicationEngine> m_engine; ///< The engine used to register QML files
        GuiControllerBackend* m_backend; ///< The backend used in the QML to control the behaviour of the simulation
};
} // namespace stride
