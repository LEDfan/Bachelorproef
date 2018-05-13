#pragma once

#include "GuiControllerBackend.h"
#include <QtCore/QObject>
#include <gengeopop/GeoGrid.h>
#include <thread>

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <sim/BaseController.h>

namespace stride {

class GuiController : public BaseController
{
public:
        GuiController(const boost::property_tree::ptree& configPt);
        GuiController(const GuiController& b) = delete;
        GuiController& operator=(const GuiController& b) = delete;

        ~GuiController();

        void Control() override;

        std::shared_ptr<QQmlApplicationEngine> getEngine();

private:
        std::shared_ptr<int>                   m_argc;
        std::shared_ptr<QGuiApplication>       m_app;
        std::shared_ptr<QQmlApplicationEngine> m_engine;
        std::shared_ptr<GuiControllerBackend>  m_backend;
};
} // namespace stride
