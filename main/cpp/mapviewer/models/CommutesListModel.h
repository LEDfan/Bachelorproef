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

#include <QtCore/QAbstractListModel>
#include <gengeopop/Location.h>

/**
 * A list model that implements data fetching and setting to display a list of commutes.
 */
class CommutesListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        CommutesListModel(QObject* parent = nullptr);

        // Get the data at the index with role
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        // Returns the ammount of rows, these are the commutes
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        // Returns the amount of columns
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        // Get the columns this model provides
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE
        /**
         * Set the commutes we wan to show in this model.
         */
        void SetCommutes(std::set<std::shared_ptr<gengeopop::Location>> locations);

signals:
        /**
         * Sends a signal to listeners whether or not there are commutes.
         * This can be used to hide a list when nothing needs to be shown for example.
         * @param status If the model contains commutes
         */
        void HasCommutes(bool status);

private:
        /**
         * The roles of the columns in the list
         */
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                Destination,
                Amount
        };

        std::shared_ptr<gengeopop::Location> m_location; ///< The location we show the commutes of
};
