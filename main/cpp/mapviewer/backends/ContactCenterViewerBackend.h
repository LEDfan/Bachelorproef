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

/**
 * This backend sets the correct text in the GUI when a new contacteCenter is shown.
 */
class ContactCenterViewerBackend : public QObject
{
        Q_OBJECT

public:
        explicit ContactCenterViewerBackend(QObject* parent = nullptr);

        Q_INVOKABLE
        /**
         * Changes the texts and table in the viewer to show the given contactCenter.
         * @param contactCenter The location to show in the viewer.
         */
        void ShowContactCenter(std::shared_ptr<gengeopop::ContactCenter> contactCenter);
};
