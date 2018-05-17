#include "GuiController.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <mapviewer/backends/ContactCenterViewerBackend.h>

namespace stride {

GuiController::GuiController(const boost::property_tree::ptree& configPt)
    : BaseController(configPt), m_argc(std::make_shared<int>(0)),
      m_app(std::make_shared<QGuiApplication>(*m_argc, nullptr)), m_engine(std::make_shared<QQmlApplicationEngine>()),
      m_backend(std::make_shared<GuiControllerBackend>(m_runner))
{
        m_engine->load(QUrl(QStringLiteral("qrc:/controllermain.qml")));
        m_engine->rootContext()->setContextProperty("backend", m_backend.get());
}

void GuiController::Control() { m_app->exec(); }

GuiController::~GuiController() {}

std::shared_ptr<QQmlApplicationEngine> GuiController::GetEngine() { return m_engine; }
} // namespace stride
