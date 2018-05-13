#pragma once

#include "models/CommutesListModel.h"
#include "models/ContactCenterListModel.h"
#include "models/ContactPoolListModel.h"
#include <QtCore/QObject>
#include <QtQml/QQmlApplicationEngine>
#include <gengeopop/GeoGrid.h>
#include <thread>

class Visualizer
{
public:
        Visualizer(std::shared_ptr<QQmlApplicationEngine> engine = nullptr);
        Visualizer(const Visualizer& b) = delete;
        Visualizer& operator=(const Visualizer& b) = delete;

        /**
         * Shows the given geogrid in the visualizer. Places the markers.
         * @param grid The grid we want to show
         */
        void SetGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid);

        void ForceUpdateMarkers();

        bool IsReady() const;

        /**
         * Block until the visualizer is closed.
         */
        void Join();

private:
        void RegisterQML(bool usingExistingEngine = false);

        QObject*                                m_rootContext = nullptr; ///< The root context of the main QML file
        QObject*                                m_qmlBackend  = nullptr; ///< The root context of the main QML file
        std::unique_ptr<std::thread>            m_thread; ///< The thread that the visualizer QT application is run on
        bool                                    m_setGrid = false; ///< If the grid was set for the visualizer
        std::shared_ptr<QQmlApplicationEngine>  m_engine;
        std::shared_ptr<ContactCenterListModel> m_ccModel;
        std::shared_ptr<ContactPoolListModel>   m_cpModel;
        std::shared_ptr<CommutesListModel>      m_commutesModel;
};
