#include "ContactPoolListModel.h"
#include <iostream>

int ContactPoolListModel::rowCount(const QModelIndex& /*parent*/) const { return static_cast<int>(m_pools.size()); }

QVariant ContactPoolListModel::data(const QModelIndex& index, int role) const
{
        auto pool = m_pools[index.row()];
        switch (role) {
        case Roles::IDRole: return QString::number(pool->GetID());
        case Roles::Capacity: return QString::number(pool->GetCapacity());
        case Roles::UsedCapacity: return QString::number(pool->GetUsedCapacity());
        default: break;
        }
        return QVariant();
}

ContactPoolListModel::ContactPoolListModel(QObject* parent) : QAbstractListModel(parent), m_pools() {}

QHash<int, QByteArray> ContactPoolListModel::RoleNames() const
{
        QHash<int, QByteArray> roles;
        roles[Roles::IDRole]       = "id";
        roles[Roles::Capacity]     = "capacity";
        roles[Roles::UsedCapacity] = "usedCapacity";
        return roles;
}

int ContactPoolListModel::columnCount(const QModelIndex& /*parent*/) const { return 2; }

void ContactPoolListModel::SetPools(std::shared_ptr<gengeopop::ContactCenter> loc)
{
        auto oldAmtRows = static_cast<int>(m_pools.size());
        m_pools         = loc->GetPools();
        int diff        = static_cast<int>(loc->GetPools().size()) - oldAmtRows;
        if (diff < 0) {
                beginRemoveRows(QModelIndex(), oldAmtRows, diff);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, static_cast<int>(m_pools.size()));
                endInsertRows();
        }
        int commonRows = std::min(oldAmtRows, static_cast<int>(m_pools.size()));
        dataChanged(createIndex(0, 0), createIndex(commonRows, 2));
}
