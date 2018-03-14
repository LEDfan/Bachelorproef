#include "backend.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QtQml>

int main(int argc, char* argv[])
{
        QGuiApplication app(argc, argv);
        qmlRegisterType<BackEnd>("io.bistromatics.backend", 1, 0, "BackEnd");
        qmlRegisterType<BackEnd>("io.bistromatics.locationviewerbackend", 1, 0, "LocationViewerBackEnd");

        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
                return -1;

        return app.exec();
}
