#include "GuiController.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <visualizer/backends/ContactCenterViewerBackend.h>

GuiController::GuiController() : m_thread(nullptr)
{
        Q_INIT_RESOURCE(qml);

        auto func = [this]() {
                int             i = 0;
                QGuiApplication app(i, nullptr);

                QQmlApplicationEngine engine;

                engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
                if (engine.rootObjects().isEmpty())
                        return -1;


                return app.exec();
        };

        m_thread = std::make_unique<std::thread>(func);
}



void GuiController::Join() { m_thread->join(); }
