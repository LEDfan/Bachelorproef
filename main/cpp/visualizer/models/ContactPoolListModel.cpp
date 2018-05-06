#include "ContactPoolListModel.h"
#include <iostream>

int ContactPoolListModel::rowCount(const QModelIndex& /*parent*/) const { return m_pools.size(); }

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

QHash<int, QByteArray> ContactPoolListModel::roleNames() const
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
                beginRemoveRows(QModelIndex(), 0, -diff - 1);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, diff - 1);
                endInsertRows();
        }
        dataChanged(createIndex(0, 0), createIndex(m_pools.size(), 2));
}
