/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>

#include "GuiController.h"
#include <QtWidgets/QApplication>
#include <mapviewer/backends/ContactCenterViewerBackend.h>

namespace stride {

GuiController::GuiController(const boost::property_tree::ptree& configPt)
    : BaseController("GuiController", configPt), m_argc(std::make_shared<int>(0)),
      m_app(std::make_unique<QApplication>(*m_argc, nullptr)), m_engine(std::make_unique<QQmlApplicationEngine>()),
      m_backend(nullptr)
{
        qmlRegisterType<GuiControllerBackend>("io.bistromatics.backend", 1, 0, "Backend");
        m_engine->load(QUrl(QStringLiteral("qrc:/controllermain.qml")));
        QObject* rootContext = m_engine->rootObjects()[0];
        QObject* qmlBackend  = rootContext->findChild<QObject*>("backend");
        if (qmlBackend) {
                m_backend = qobject_cast<GuiControllerBackend*>(qmlBackend);
                m_backend->SetRunner(m_runner);
        }
}

void GuiController::Control() { m_app->exec(); }

GuiController::~GuiController() {}

QQmlApplicationEngine* GuiController::GetEngine() { return m_engine.get(); }
} // namespace stride
