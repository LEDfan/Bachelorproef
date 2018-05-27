#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>

#include "Map.h"

int main(int, char* [])
{
        Q_INIT_RESOURCE(qml);
        int             i = 0;
        QGuiApplication app(i, nullptr);
        auto            engine = std::make_unique<QQmlApplicationEngine>();
        Map             m(engine.get());
        app.exec();
}
