#include "Visualizer.h"
#include "backend.h"
#include "backends/LocationViewerBackend.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <visualizer/backends/ContactCenterViewerBackend.h>
#include <visualizer/models/ContactCenterListModel.h>
#include <visualizer/models/ContactPoolListModel.h>
#include <thread>

Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::Location>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::GeoGrid>)
Q_DECLARE_METATYPE(std::set<std::shared_ptr<gengeopop::Location>>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::ContactCenter>)

Visualizer::Visualizer() {
    Q_INIT_RESOURCE(qml);

    m_bridge =  new LogicBridge;

    auto func = [this](){
        int i = 0;
        QGuiApplication app(i,nullptr);

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
        QQmlApplicationEngine engine;

        ContactCenterListModel ccModel;
        ContactPoolListModel   cpModel;
        engine.rootContext()->setContextProperty("ccModel", &ccModel);
        engine.rootContext()->setContextProperty("cpModel", &cpModel);
        engine.rootContext()->setContextProperty("bridge", m_bridge);
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;

        // Save the root contect
        m_rootContext = engine.rootObjects()[0];

        return app.exec();
    };

    m_thread = std::make_unique<std::thread>(func);
}

void Visualizer::forceUpdateMarkers() {
    auto backend = m_rootContext->findChild<QObject*>("backend");
    // Force to update colors of sickness
}

void Visualizer::setGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid) {
    QObject* backend = m_rootContext->findChild<QObject*>("backend");
    Backend* backendClass = qobject_cast<Backend*>(backend);
    backendClass->SetGeoGrid(grid);

}

void Visualizer::join() {
    m_thread->join();

}
