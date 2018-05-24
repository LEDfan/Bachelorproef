#pragma once

#include "models/CommutesListModel.h"
#include "models/ContactCenterListModel.h"
#include "models/ContactPoolListModel.h"
#include <QtCore/QObject>
#include <QtQml/QQmlApplicationEngine>
#include <gengeopop/GeoGrid.h>
#include <thread>

class Map
{
public:
        /// Construct the Map
        /// @param engine An optional engine to use for the QML. If it is not provided, a new one will be created
        Map(std::shared_ptr<QQmlApplicationEngine> engine);
        Map(const Map& b) = delete;
        Map& operator=(const Map& b) = delete;
        ~Map();

        /**
         * Shows the given geogrid in the mapviewer. Places the markers.
         * @param grid The grid we want to show
         */
        void SetGeoGrid(std::shared_ptr<gengeopop::GeoGrid> grid);

        /// Force the markers on the map to be updated
        void ForceUpdateMarkers();

        /// Returns if the Gui Application is successfully started on a different thread,
        /// only used when no existing engine was passed in the constructor
        bool IsReady() const;

private:
        void RegisterQML();

        QObject*                                m_rootContext = nullptr; ///< The root context of the main QML file
        QObject*                                m_qmlBackend  = nullptr; ///< The root context of the main QML file
        std::unique_ptr<std::thread>            m_thread; ///< The thread that the mapviewer QT application is run on
        bool                                    m_setGrid = false; ///< If the grid was set for the mapviewer
        std::shared_ptr<QQmlApplicationEngine>  m_engine;          ///< The engine used to load the QML
        std::shared_ptr<ContactCenterListModel> m_ccModel;         ///< A QML model for displaying the ContactCenters
        std::shared_ptr<ContactPoolListModel>   m_cpModel;         ///< A QML model for displaying the ContactPools
        std::shared_ptr<CommutesListModel>      m_commutesModel;   ///< A QML model for displaying the Commutes
};
