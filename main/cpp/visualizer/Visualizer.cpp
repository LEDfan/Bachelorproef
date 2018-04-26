#include "Visualizer.h"
#include "backend.h"
#include "backends/LocationViewerBackend.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <models/CommutesListModel.h>
#include <models/ContactCenterListModel.h>
#include <models/ContactPoolListModel.h>
#include <visualizer/backends/ContactCenterViewerBackend.h>

Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::Location>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::GeoGrid>)
Q_DECLARE_METATYPE(std::set<std::shared_ptr<gengeopop::Location>>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::ContactCenter>)

Visualizer::Visualizer() : m_thread(nullptr)
{
        Q_INIT_RESOURCE(qml);

        auto func = [this]() {
                int             i = 0;
                QGuiApplication app(i, nullptr);

                qmlRegisterType<Backend>("io.bistromatics.backend", 1, 0, "Backend");
                qmlRegisterType<LocationViewerBackend>("io.bistromatics.locationviewerbackend", 1, 0,
                                                       "LocationViewerBackend");
                qmlRegisterType<ContactCenterViewerBackend>("io.bistromatics.contactcenterviewerbackend", 1, 0,
                                                            "ContactCenterViewerBackend");
                qmlRegisterType<ContactCenterListModel>("io.bistromatics.contactcenterlistmodel", 1, 0,
                                                        "ContactCenterListModel");

                qRegisterMetaType<std::shared_ptr<gengeopop::Location>>();
                qRegisterMetaType<std::shared_ptr<gengeopop::GeoGrid>>();
                qRegisterMetaType<std::vector<std::shared_ptr<gengeopop::ContactCenter>>>();
                qRegisterMetaType<std::set<std::shared_ptr<gengeopop::Location>>>();
                QQmlApplicationEngine engine;

                ContactCenterListModel ccModel;
                ContactPoolListModel   cpModel;
                CommutesListModel      commutesModel;
                engine.rootContext()->setContextProperty("ccModel", &ccModel);
                engine.rootContext()->setContextProperty("cpModel", &cpModel);
                engine.rootContext()->setContextProperty("commutesModel", &commutesModel);
                engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
                if (engine.rootObjects().isEmpty())
                        return -1;

                // Save the root contect
                m_rootContext = engine.rootObjects()[0];

                return app.exec();
        };

        m_thread = std::make_unique<std::thread>(func);
}

void Visualizer::ForceUpdateMarkers()
{
        QObject* backend      = m_rootContext->findChild<QObject*>("backend");
        if(backend != nullptr){
            Backend* backendClass = qobject_cast<Backend*>(backend);
            backendClass->UpdateAllHealthColors();
        }
}

void Visualizer::SetGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid)
{
        QObject* backend      = m_rootContext->findChild<QObject*>("backend");
        if(backend != nullptr) {
            Backend *backendClass = qobject_cast<Backend *>(backend);
            backendClass->SetGeoGrid(grid);
        }
}

void Visualizer::Join() { m_thread->join(); }
