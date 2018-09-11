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

#include "ContactPoolListModel.h"
#include <gengeopop/ContactCenter.h>
#include <iostream>

int ContactPoolListModel::rowCount(const QModelIndex& /*parent*/) const { return m_pools.size(); }

QVariant ContactPoolListModel::data(const QModelIndex& index, int role) const
{
        auto pool = m_pools[index.row()];
        switch (role) {
        case Roles::IDRole: return QString::number(pool->GetId());
        case Roles::UsedCapacity: return QString::number(pool->GetUsedCapacity());
        default: break;
        }
        return QVariant();
}

ContactPoolListModel::ContactPoolListModel(QObject* parent) : QAbstractListModel(parent), m_pools() {}

QHash<int, QByteArray> ContactPoolListModel::roleNames() const
{
        QHash<int, QByteArray> roles;
        roles[Roles::IDRole]       = "id";
        roles[Roles::UsedCapacity] = "usedCapacity";
        return roles;
}

int ContactPoolListModel::columnCount(const QModelIndex& /*parent*/) const { return 1; }

void ContactPoolListModel::SetPools(std::shared_ptr<gengeopop::ContactCenter> loc)
{
        auto oldAmtRows = static_cast<int>(m_pools.size());
        m_pools         = loc->GetPools();
        int diff        = static_cast<int>(loc->GetPools().size()) - oldAmtRows;
        if (diff < 0) {
                beginRemoveRows(QModelIndex(), 0, -diff - 1);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, diff - 1);
                endInsertRows();
        }
        dataChanged(createIndex(0, 0), createIndex(m_pools.size(), 1));
}
