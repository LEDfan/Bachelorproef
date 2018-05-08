#pragma once

#include <QtCore/QObject>
#include <gengeopop/GeoGrid.h>
#include <thread>

#include <sim/BaseController.h>

namespace stride {

class GuiController : public BaseController
{
public:
        GuiController(const boost::property_tree::ptree& configPt);
        GuiController(const GuiController& b) = delete;
        GuiController& operator=(const GuiController& b) = delete;

        void Control() override;

        /**
         * Block until the visualizer is closed.
         */
        void Join();

private:
        std::unique_ptr<std::thread> m_thread; ///< The thread that the visualizer QT application is run on
};
} // namespace stride
