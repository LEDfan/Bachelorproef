#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>

#include "GuiController.h"
#include <mapviewer/backends/ContactCenterViewerBackend.h>

namespace stride {

GuiController::GuiController(const boost::property_tree::ptree& configPt)
    : BaseController(configPt), m_argc(std::make_shared<int>(0)),
      m_app(std::make_unique<QGuiApplication>(*m_argc, nullptr)), m_engine(std::make_unique<QQmlApplicationEngine>()),
      m_backend(nullptr)
{
        qmlRegisterType<GuiControllerBackend>("io.bistromatics.backend", 1, 0, "Backend");
        m_engine->load(QUrl(QStringLiteral("qrc:/controllermain.qml")));
        QObject* rootContext = m_engine->rootObjects()[0];
        QObject* qmlBackend  = rootContext->findChild<QObject*>("backend");
        if (qmlBackend) {
                m_backend = qobject_cast<GuiControllerBackend*>(qmlBackend);
                m_backend->SetRunner(m_runner);
        }
}

void GuiController::Control() { m_app->exec(); }

GuiController::~GuiController() {}

QQmlApplicationEngine* GuiController::GetEngine() { return m_engine.get(); }
} // namespace stride
