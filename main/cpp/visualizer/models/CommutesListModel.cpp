#include "CommutesListModel.h"
#include <iostream>

int CommutesListModel::rowCount(const QModelIndex& /*parent*/) const {
        return ((m_location != nullptr) ? static_cast<int>(m_location->getOutgoingCommuningCities().size()) : 0);
}

QVariant CommutesListModel::data(const QModelIndex& index, int role) const
{
        auto commute = m_location->getOutgoingCommuningCities()[index.row()];
        switch (role) {
        case Roles::Destination: return QString::fromStdString(commute.first->getName());
        case Roles::Amount: return QString::number(commute.second);
        default: break;
        }
        return QVariant();
}

CommutesListModel::CommutesListModel(QObject* parent) : QAbstractListModel(parent) {}

QHash<int, QByteArray> CommutesListModel::roleNames() const
{
        QHash<int, QByteArray> roles;
        roles[Roles::Destination]     = "destination";
        roles[Roles::Amount] = "amount";
        return roles;
}

int CommutesListModel::columnCount(const QModelIndex& /*parent*/) const { return 2; }

void CommutesListModel::setCommutes(std::set<std::shared_ptr<gengeopop::Location> > locations) {
        auto oldAmtRows = m_location == nullptr ? 0 : static_cast<int>(m_location->getOutgoingCommuningCities().size());
        if(locations.size() != 1){
                m_location = nullptr;
                beginRemoveRows(QModelIndex(), 0, oldAmtRows);
                endRemoveRows();
                hasCommutes(false);
                return;
        }
        hasCommutes(true);
        m_location         = *locations.begin();
        int newAmount = static_cast<int>(m_location->getOutgoingCommuningCities().size());
        int diff        = newAmount - oldAmtRows;
        if (diff < 0) {
                beginRemoveRows(QModelIndex(), 0, -diff);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, diff-1);
                endInsertRows();
        }
        dataChanged(createIndex(0, 0), createIndex(newAmount-1, 2));
}
