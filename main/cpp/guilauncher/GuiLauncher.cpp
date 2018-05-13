#include "GuiLauncher.h"
#include "Launcher.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <guicontroller/GuiControllerBackend.h>
#include <visualizer/backends/ContactCenterViewerBackend.h>

GuiLauncher::GuiLauncher() : m_thread(nullptr), m_launcher()
{
        Q_INIT_RESOURCE(qml);
        Q_INIT_RESOURCE(launcherqml);
        Q_INIT_RESOURCE(controllerqml);

        auto func = [this]() {
                int                   i = 0;
                QGuiApplication       app(i, nullptr);
                QQmlApplicationEngine engine;
                qmlRegisterType<Launcher>("io.bistromatics.launcher", 1, 0, "Launcher");

                engine.rootContext()->setContextProperty("launcher", &m_launcher);
                engine.load(QUrl(QStringLiteral("qrc:/launchermain.qml")));
                if (engine.rootObjects().isEmpty())
                        return -1;

                return app.exec();
        };
        m_thread = std::make_unique<std::thread>(func);
}

void GuiLauncher::StartSimulation()
{
        // Wait for the launcher to close
        m_thread->join();
        // Launch using the settings
        m_launcher.launchIfSet();
}
