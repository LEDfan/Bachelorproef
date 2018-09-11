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

#pragma once

#include <QObject>
#include <QString>
#include <gengeopop/GeoGrid.h>
#include <set>

/**
 * This backend sets the correct info in the GUI of the location that is being shown.
 */
class LocationViewerBackend : public QObject
{
        Q_OBJECT

public:
        explicit LocationViewerBackend(QObject* parent = nullptr);

        Q_INVOKABLE
        /**
         * Adapts the text and tableView model to show the correct info about the location.
         * @param location The location to show in the viewer.
         */
        void ShowLocations(std::set<std::shared_ptr<gengeopop::Location>> location);

        // Updates the infected count
        Q_INVOKABLE
        void UpdateInfected();

signals:
        void ContactCenterSelected();

private:
        /**
         * @param id
         * @return the name of the Province that coressponds with the given NIS id
         */
        QString NISToProvince(unsigned int id);

        /// The locations we are displaying
        std::set<std::shared_ptr<gengeopop::Location>> m_locations;
};
