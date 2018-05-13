#include "Visualizer.h"
#include "backend.h"
#include "backends/LocationViewerBackend.h"
#include "models/CommutesListModel.h"
#include "models/ContactCenterListModel.h"
#include "models/ContactPoolListModel.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QtQml>
#include <visualizer/backends/ContactCenterViewerBackend.h>

Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::Location>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::GeoGrid>)
Q_DECLARE_METATYPE(std::set<std::shared_ptr<gengeopop::Location>>)
Q_DECLARE_METATYPE(std::shared_ptr<gengeopop::ContactCenter>)

Visualizer::Visualizer(std::shared_ptr<QQmlApplicationEngine> engine)
    : m_rootContext(nullptr), m_thread(nullptr), m_setGrid(false), m_engine(engine), m_ccModel(nullptr),
      m_cpModel(nullptr), m_commutesModel(nullptr)
{
        if (m_engine) {
                RegisterQML(true);
        } else {
                auto application = [this]() {
                        int             i = 0;
                        QGuiApplication app(i, nullptr);
                        m_engine = std::make_shared<QQmlApplicationEngine>();
                        RegisterQML();
                        if (m_engine->rootObjects().isEmpty())
                                return -1;

                        return app.exec();
                };
                m_thread = std::make_unique<std::thread>(application);
        }
}

void Visualizer::RegisterQML(bool usingExistingEngine)
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
        int offset = 0;
        if (usingExistingEngine) {
                offset = 1;
        }
        m_rootContext = m_engine->rootObjects()[offset];
        m_qmlBackend  = m_rootContext->findChild<QObject*>("backend");
}

void Visualizer::ForceUpdateMarkers()
{
        QObject* backend = m_rootContext->findChild<QObject*>("backend");
        if (backend != nullptr && m_setGrid) {
                Backend* backendClass = qobject_cast<Backend*>(backend);
                backendClass->UpdateAllHealthColors();
        }
}

void Visualizer::SetGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid)
{
        if (m_qmlBackend != nullptr && grid != nullptr) {
                Backend* backendClass = qobject_cast<Backend*>(m_qmlBackend);
                backendClass->SetGeoGrid(grid);
                m_setGrid = true;
        }
}

void Visualizer::Join() {}

bool Visualizer::IsReady() const { return m_rootContext != nullptr && m_qmlBackend != nullptr; }
