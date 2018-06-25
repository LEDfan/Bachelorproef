#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <QtWidgets/QApplication>

#include "Map.h"

int main(int, char* [])
{
        Q_INIT_RESOURCE(qml);
        int          i = 0;
        QApplication app(i, nullptr);
        auto         engine = std::make_unique<QQmlApplicationEngine>();
        Map          m(engine.get());
        app.exec();
}
