#include "GuiController.h"
#include "GuiControllerBackend.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <visualizer/backends/ContactCenterViewerBackend.h>

namespace stride {

GuiController::GuiController(const boost::property_tree::ptree& configPt) : BaseController(configPt), m_thread(nullptr)
{

        auto func = [this]() {
                int             i = 0;
                QGuiApplication app(i, nullptr);

                QQmlApplicationEngine engine;

                GuiControllerBackend backend(m_runner);
                engine.load(QUrl(QStringLiteral("qrc:/controllermain.qml")));
                engine.rootContext()->setContextProperty("backend", &backend);
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
