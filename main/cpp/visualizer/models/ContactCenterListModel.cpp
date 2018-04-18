//
// Created by niels on 15.03.18.
//

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
                beginRemoveRows(QModelIndex(), 0, -diff);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, diff - 1);
                endInsertRows();
        }
        int commonRows = std::min(oldAmtRows, (unsigned int)m_centers.size());
        dataChanged(createIndex(0, 0), createIndex(commonRows, 2));
}

void ContactCenterListModel::HandleClickRow(unsigned int row)
{
        try {
                auto cc = m_centers.at(row);
                emit ContactCenterSelected(cc);
        } catch (std::exception& e) {
        }
}
