#include "GuiController.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <visualizer/backends/ContactCenterViewerBackend.h>

namespace stride {

GuiController::GuiController(const boost::property_tree::ptree& configPt) : BaseController(configPt), m_thread(nullptr)
{
        auto func = []() {
                int             i = 0;
                QGuiApplication app(i, nullptr);

                QQmlApplicationEngine engine;

                engine.load(QUrl(QStringLiteral("qrc:/controllermain.qml")));
                if (engine.rootObjects().isEmpty())
                        return -1;

                return app.exec();
        };

        m_thread = std::make_unique<std::thread>(func);
}

void GuiController::Join() { m_thread->join(); }

void GuiController::Control() {}

GuiController::~GuiController() {
    Join();

}
    // TODO

} // namespace stride
