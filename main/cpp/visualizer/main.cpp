#include "backend.h"
#include "backends/LocationViewerBackend.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QtQml>

Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::Location>)

int main(int argc, char* argv[])
{
        QGuiApplication app(argc, argv);
        qmlRegisterType<BackEnd>("io.bistromatics.backend", 1, 0, "BackEnd");
        qmlRegisterType<LocationViewerBackend>("io.bistromatics.locationviewerbackend", 1, 0, "LocationViewerBackend");

        qRegisterMetaType<std::shared_ptr<gengeopop::Location>>();
        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
                return -1;

        return app.exec();
}
