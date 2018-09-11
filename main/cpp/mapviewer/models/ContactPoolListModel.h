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

#include <QtCore/QAbstractListModel>
#include <gengeopop/Location.h>
#include <pool/ContactPool.h>

/*
 * A Qt list model that implements data fetching and -setting to display a list of contactpools.
 */
class ContactPoolListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        ContactPoolListModel(QObject* parent = nullptr);

        /**
         * @param index the index we want to get the data of.
         * @param role the role we want the data of
         */
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        // Returns the amount of row, the amount of contactpools
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        // Returns the amount of columns
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        // Returns the available columns of the model
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE
        /**
         * Sets the contactpool we want to show in the model.
         * @param cc the contactecenters we want to show the ContactPools of.
         */
        void SetPools(std::shared_ptr<gengeopop::ContactCenter> cc);

private:
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                UsedCapacity
        };

        std::vector<stride::ContactPool*> m_pools;
};
