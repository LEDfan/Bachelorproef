#include "GuiLauncher.h"
#include "Launcher.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <visualizer/backends/ContactCenterViewerBackend.h>

GuiLauncher::GuiLauncher() : m_thread(nullptr)
{
        Q_INIT_RESOURCE(qml);

        auto func = [this]() {
                int             i = 0;
                QGuiApplication app(i, nullptr);
                qmlRegisterType<Launcher>("io.bistromatics.launcher", 1, 0,
                                                        "Launcher");
                QQmlApplicationEngine engine;

                engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
                if (engine.rootObjects().isEmpty())
                        return -1;


                return app.exec();
        };

        m_thread = std::make_unique<std::thread>(func);
}



void GuiLauncher::Join() { m_thread->join(); }