#pragma once

#include "GuiControllerBackend.h"
#include <gengeopop/GeoGrid.h>
#include <sim/BaseController.h>

#include <QtCore/QObject>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <thread>

namespace stride {

class GuiController : public BaseController
{
public:
        GuiController(const boost::property_tree::ptree& configPt);
        GuiController(const GuiController& b) = delete;
        GuiController& operator=(const GuiController& b) = delete;

        ~GuiController();

        void Control() override;

        std::shared_ptr<QQmlApplicationEngine> GetEngine();

private:
        std::shared_ptr<int>                   m_argc;
        std::shared_ptr<QGuiApplication>       m_app;
        std::shared_ptr<QQmlApplicationEngine> m_engine;
        GuiControllerBackend*                  m_backend;
};
} // namespace stride
