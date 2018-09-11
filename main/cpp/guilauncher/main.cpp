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
