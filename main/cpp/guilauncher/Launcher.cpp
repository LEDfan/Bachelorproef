#include "Launcher.h"
#include "GuiLauncher.h"
#include <QtCore/QUrl>
#include <guicontroller/GuiController.h>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <iostream>
#include <sim/BaseController.h>
#include <sim/CliController.h>
#include <util/FileSys.h>
#include <util/TimeStamp.h>
#include <viewers/AdoptedViewer.h>
#include <viewers/CliViewer.h>
#include <viewers/InfectedViewer.h>
#include <viewers/MapViewer.h>
#include <viewers/PersonsViewer.h>
#include <viewers/SummaryViewer.h>

Launcher::Launcher()
    : m_configPath(), m_showVisualizer(false), m_showMapViewer(false), m_showAdoptedViewer(false),
      m_showCliViewer(false), m_showInfectedViewer(false), m_showPersonsViewer(false), m_showSummaryViewer(false)
{
}

void Launcher::launchIfSet()
{
        if (!m_setToLaunch) {
                return;
        }

        // -----------------------------------------------------------------------------------------
        // Get configuration and path with overrides (if any).
        // -----------------------------------------------------------------------------------------
//        configPt = stride::util::FileSys::ReadPtreeFile(m_configPath);
        if (m_configPt.get<std::string>("run.output_prefix", "").empty()) {
                m_configPt.put<std::string>("run.output_prefix", stride::util::TimeStamp().ToTag().append("/"));
        }

        m_configPt.sort();
        std::shared_ptr<stride::BaseController> controller = nullptr;
        std::shared_ptr<QQmlApplicationEngine>  engine     = nullptr;

        if (m_controller == 1) {
                auto guiController = std::make_shared<stride::GuiController>(configPt);
                engine             = guiController->GetEngine();
                controller         = guiController;
        } else {
                controller = std::make_shared<stride::CliController>(m_configPt);
        }
        if (m_showMapViewer) {
                controller->RegisterViewer<stride::viewers::MapViewer>(controller->GetLogger(), engine);
        }
        if (m_showAdoptedViewer) {
                controller->RegisterViewer<stride::viewers::AdoptedViewer>(controller->GetOutputPrefix());
        }
        if (m_showCliViewer) {
                controller->RegisterViewer<stride::viewers::CliViewer>(controller->GetLogger());
        }
        if (m_showInfectedViewer) {
                controller->RegisterViewer<stride::viewers::InfectedViewer>(controller->GetOutputPrefix());
        }
        if (m_showPersonsViewer) {
                controller->RegisterViewer<stride::viewers::PersonsViewer>(controller->GetOutputPrefix());
        }
        if (m_showSummaryViewer) {
                controller->RegisterViewer<stride::viewers::SummaryViewer>(controller->GetOutputPrefix());
        }
        controller->Control();
}

void Launcher::setConfigPath(QString file)
{
        QUrl info(file);
        m_configPath = info.toLocalFile().toStdString();
        m_configPt = stride::util::FileSys::ReadPtreeFile(m_configPath);
        UpdateConfigForm();
}

void Launcher::setConfig(bool showMapViewer, bool showAdoptedViewer, bool showCliViewer, bool showInfectedViewer,
                         bool showPersonsViewer, bool showSummaryViewer)
{
        m_showMapViewer      = showMapViewer;
        m_showAdoptedViewer  = showAdoptedViewer;
        m_showCliViewer      = showCliViewer;
        m_showInfectedViewer = showInfectedViewer;
        m_showPersonsViewer  = showPersonsViewer;
        m_showSummaryViewer  = showSummaryViewer;
}

void Launcher::setToLaunch() { m_setToLaunch = true; }

void Launcher::setController(int index) { m_controller = index; }

void Launcher::UpdateConfigForm() {
        m_configEditor.r0->setProperty("value", 100000);
}

void Launcher::SetRootObject(QObject* rootObject) {
        m_configEditor.r0 = rootObject->findChild<QObject*>("inputR0");

}
