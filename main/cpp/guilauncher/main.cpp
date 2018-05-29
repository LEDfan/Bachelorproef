#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>

#include "Launcher.h"

void LaunchGui()
{

        Q_INIT_RESOURCE(qml);
        Q_INIT_RESOURCE(launcherqml);
        Q_INIT_RESOURCE(controllerqml);

        int      i = 0;
        Launcher launcher;
        { // Makes sure the QGuiApplication is destructed when launcher.launch() is called
                QGuiApplication       app(i, nullptr);
                QQmlApplicationEngine engine;
                qmlRegisterType<Launcher>("io.bistromatics.launcher", 1, 0, "Launcher");

                engine.rootContext()->setContextProperty("launcher", &launcher);
                engine.load(QUrl(QStringLiteral("qrc:/launchermain.qml")));
                if (engine.rootObjects().isEmpty())
                        return;

                launcher.SetRootObject(engine.rootObjects().first());

                app.exec();
        }
        // Wait for the launcher to close
        launcher.Launch();
}

int main(int, char* [])
{
        int crashed = 0;
        while (crashed >= 0 && crashed < 5) {
                try {
                        LaunchGui();
                        crashed = -1;
                } catch (...) {
                        crashed++;
                }
        }
}
