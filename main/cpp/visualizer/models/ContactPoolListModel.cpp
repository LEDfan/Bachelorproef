#include "ContactPoolListModel.h"
#include <iostream>

int ContactPoolListModel::rowCount(const QModelIndex& /*parent*/) const { return m_pools.size(); }

QVariant ContactPoolListModel::data(const QModelIndex& index, int role) const
{
        auto pool = m_pools[index.row()];
        switch (role) {
        case Roles::IDRole: return QString::number(pool->getID());
        case Roles::Capacity: return QString::number(pool->getCapacity());
        case Roles::UsedCapacity: return QString::number(pool->getUsedCapacity());
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

void ContactPoolListModel::setPools(std::shared_ptr<gengeopop::ContactCenter> loc)
{
        unsigned int oldAmtRows = m_pools.size();
        m_pools                 = loc->GetPools();
        int diff                = loc->GetPools().size() - oldAmtRows;
        if (diff < 0) {
                beginRemoveRows(QModelIndex(), oldAmtRows, -diff);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, m_pools.size());
                endInsertRows();
        }
        int commonRows = std::min(oldAmtRows, (unsigned int)m_pools.size());
        dataChanged(createIndex(0, 0), createIndex(commonRows, 2));
}
