#include "GuiLauncher.h"
#include "Launcher.h"
#include <guicontroller/GuiControllerBackend.h>
#include <mapviewer/backends/ContactCenterViewerBackend.h>

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>

void GuiLauncher::Start()
{
        Q_INIT_RESOURCE(qml);
        Q_INIT_RESOURCE(launcherqml);
        Q_INIT_RESOURCE(controllerqml);
        Launcher launcher;

        int                   i = 0;
        QGuiApplication       app(i, nullptr);
        QQmlApplicationEngine engine;
        qmlRegisterType<Launcher>("io.bistromatics.launcher", 1, 0, "Launcher");

        engine.rootContext()->setContextProperty("launcher", &launcher);
        engine.load(QUrl(QStringLiteral("qrc:/launchermain.qml")));
        if (engine.rootObjects().isEmpty())
                return;

        launcher.SetRootObject(engine.rootObjects().first());

        app.exec();
        // Wait for the launcher to close
        launcher.Launch();
}
