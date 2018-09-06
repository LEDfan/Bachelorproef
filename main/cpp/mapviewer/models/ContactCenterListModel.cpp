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

#include "ContactCenterListModel.h"
#include <algorithm>
#include <gengeopop/Location.h>
#include <iostream>

int ContactCenterListModel::rowCount(const QModelIndex& /*parent*/) const { return m_centers.size(); }

QVariant ContactCenterListModel::data(const QModelIndex& index, int role) const
{
        auto center = m_centers[index.row()];
        switch (role) {
        case Roles::IDRole: break;

        case Roles::TypeRole: return QString::fromStdString(center->GetType());

        case Roles::PoolSize: return QString::number(center->GetPoolSize());

        case Roles::MaxPools: return QString::number(center->GetMaxPools());
        }
        return QVariant();
}

ContactCenterListModel::ContactCenterListModel(QObject* parent) : QAbstractListModel(parent), m_centers() {}

QHash<int, QByteArray> ContactCenterListModel::roleNames() const
{
        QHash<int, QByteArray> roles;
        roles[Roles::IDRole]   = "id";
        roles[Roles::TypeRole] = "type";
        roles[Roles::PoolSize] = "poolSize";
        roles[Roles::MaxPools] = "maxPools";
        return roles;
}

int ContactCenterListModel::columnCount(const QModelIndex& /*parent*/) const { return 2; }

void ContactCenterListModel::SetCenters(std::set<std::shared_ptr<gengeopop::Location>> locs)
{
        unsigned int oldAmtRows = m_centers.size();
        m_centers.clear();
        for (auto loc : locs) {
                auto centers = loc->GetContactCenters();
                for (auto center : centers) {
                        m_centers.push_back(center);
                }
        }

        int diff = m_centers.size() - oldAmtRows;
        if (diff < 0) {
                beginRemoveRows(QModelIndex(), 0, -diff - 1);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, diff - 1);
                endInsertRows();
        }
        dataChanged(createIndex(0, 0), createIndex(m_centers.size(), 2));
}

void ContactCenterListModel::HandleClickRow(unsigned int row)
{
        try {
                auto cc = m_centers.at(row);
                emit ContactCenterSelected(cc);
        } catch (std::exception& e) {
        }
}
