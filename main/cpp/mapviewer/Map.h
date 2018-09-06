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

#pragma once

#include "models/CommutesListModel.h"
#include "models/ContactCenterListModel.h"
#include "models/ContactPoolListModel.h"
#include <QtCore/QObject>
#include <QtQml/QQmlApplicationEngine>
#include <gengeopop/GeoGrid.h>
#include <thread>

/// A geographical map used for showing in the MapViewer
class Map
{
public:
        /// Construct the Map
        /// @param engine An optional engine to use for the QML. If it is not provided, a new one will be created
        Map(QQmlApplicationEngine* engine);
        Map(const Map& b) = delete;
        Map& operator=(const Map& b) = delete;
        ~Map();

        /**
         * Shows the given geogrid in the mapviewer. Places the markers.
         * @param grid The grid we want to show
         */
        void SetGeoGrids(std::vector<std::shared_ptr<gengeopop::GeoGrid>> grid);

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
        QQmlApplicationEngine*                  m_engine;          ///< The engine used to load the QML
        std::shared_ptr<ContactCenterListModel> m_ccModel;         ///< A QML model for displaying the ContactCenters
        std::shared_ptr<ContactPoolListModel>   m_cpModel;         ///< A QML model for displaying the ContactPools
        std::shared_ptr<CommutesListModel>      m_commutesModel;   ///< A QML model for displaying the Commutes
};
