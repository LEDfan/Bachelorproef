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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

#include "Map.h"
#include "backend.h"
#include "backends/LocationViewerBackend.h"
#include "models/CommutesListModel.h"
#include "models/ContactCenterListModel.h"
#include "models/ContactPoolListModel.h"
#include <mapviewer/backends/ContactCenterViewerBackend.h>

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <utility>

Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::Location>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::GeoGrid>)
Q_DECLARE_METATYPE(std::set<std::shared_ptr<gengeopop::Location>>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::ContactCenter>)

Map::Map(QQmlApplicationEngine* engine)
    : m_rootContext(nullptr), m_thread(nullptr), m_setGrid(false), m_engine(engine), m_ccModel(nullptr),
      m_cpModel(nullptr), m_commutesModel(nullptr)
{
        RegisterQML();
}

void Map::RegisterQML()
{
        qmlRegisterType<Backend>("io.bistromatics.backend", 1, 0, "Backend");
        qmlRegisterType<LocationViewerBackend>("io.bistromatics.locationviewerbackend", 1, 0, "LocationViewerBackend");
        qmlRegisterType<ContactCenterViewerBackend>("io.bistromatics.contactcenterviewerbackend", 1, 0,
                                                    "ContactCenterViewerBackend");
        qmlRegisterType<ContactCenterListModel>("io.bistromatics.contactcenterlistmodel", 1, 0,
                                                "ContactCenterListModel");

        qRegisterMetaType<std::shared_ptr<gengeopop::Location>>();
        qRegisterMetaType<std::shared_ptr<gengeopop::GeoGrid>>();
        qRegisterMetaType<std::vector<std::shared_ptr<gengeopop::ContactCenter>>>();
        qRegisterMetaType<std::set<std::shared_ptr<gengeopop::Location>>>();

        m_ccModel       = std::make_shared<ContactCenterListModel>();
        m_cpModel       = std::make_shared<ContactPoolListModel>();
        m_commutesModel = std::make_shared<CommutesListModel>();
        m_engine->rootContext()->setContextProperty("ccModel", m_ccModel.get());
        m_engine->rootContext()->setContextProperty("cpModel", m_cpModel.get());
        m_engine->rootContext()->setContextProperty("commutesModel", m_commutesModel.get());
        m_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

        // Save the root context
        int offset    = m_engine->rootObjects().size() - 1;
        m_rootContext = m_engine->rootObjects()[offset];
        m_qmlBackend  = m_rootContext->findChild<QObject*>("backend");
}

void Map::ForceUpdateMarkers()
{
        QObject* backend = m_rootContext->findChild<QObject*>("backend");
        if (backend != nullptr && m_setGrid) {
                Backend* backendClass = qobject_cast<Backend*>(backend);
                backendClass->UpdateAllHealthColors();
        }
}

void Map::SetGeoGrids(std::vector<std::shared_ptr<gengeopop::GeoGrid>> grids)
{
        if (m_qmlBackend != nullptr) {
                Backend* backendClass = qobject_cast<Backend*>(m_qmlBackend);
                backendClass->SetGeoGrids(grids);
                m_setGrid = true;
        }
}

bool Map::IsReady() const { return m_rootContext != nullptr && m_qmlBackend != nullptr; }

Map::~Map()
{
        if (m_thread)
                m_thread->join();
}
