//
// Created by niels on 15.03.18.
//

#include "ContactCenterListModel.h"
#include <algorithm>
#include <gengeopop/Location.h>
#include <iostream>

int ContactCenterListModel::rowCount(const QModelIndex& parent) const {
        std::cout << "Rowcount" << std::endl;
        return m_centers.size(); }

QVariant ContactCenterListModel::data(const QModelIndex& index, int role) const
{
        auto center = m_centers[index.row()];
        switch (role) {
        case Roles::IDRole: break;

        case Roles::TypeRole: return QString::fromStdString(center->getType());

        case Roles::PoolSize: return QString::number(center->getPoolSize());

        case Roles::MaxPools: return QString::number(center->getMaxPools());
        }
        return QVariant();
}

ContactCenterListModel::ContactCenterListModel(QObject* parent) : QAbstractListModel(parent) {}

QHash<int, QByteArray> ContactCenterListModel::roleNames() const
{
        QHash<int, QByteArray> roles;
        roles[Roles::IDRole]   = "id";
        roles[Roles::TypeRole] = "type";
        roles[Roles::PoolSize] = "poolSize";
        roles[Roles::MaxPools] = "maxPools";
        return roles;
}

int ContactCenterListModel::columnCount(const QModelIndex& parent) const { return 2; }

void ContactCenterListModel::setCenters(std::vector<std::shared_ptr<gengeopop::Location>> locs)
{
        unsigned int oldAmtRows = m_centers.size();
        m_centers.clear();
        for (auto loc : locs) {
                auto centers = loc->getContactCenters();
                m_centers.insert(m_centers.end(), centers.begin(), centers.end());
        }

        int diff = m_centers.size() - oldAmtRows;
        std::cout << "DIF ROWS" << diff << std::endl;
        if (diff < 0) {
                beginRemoveRows(QModelIndex(), oldAmtRows, -diff);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, diff);
                std::cout << "\t inserted: " << diff << std::endl;
                endInsertRows();
        }
        int commonRows = std::min(oldAmtRows, (unsigned int)m_centers.size());
        dataChanged(createIndex(0, 0), createIndex(commonRows, 2));
}

void ContactCenterListModel::handleClickRow(unsigned int row)
{
        try {
                auto cc = m_centers.at(row);
                emit ContactCenterSelected(cc);
        } catch (std::exception& e) {
        }
}
