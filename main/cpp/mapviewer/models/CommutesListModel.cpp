#include "CommutesListModel.h"
#include <iostream>

int CommutesListModel::rowCount(const QModelIndex& /*parent*/) const
{
        return ((m_location != nullptr) ? static_cast<int>(m_location->GetOutgoingCommuningCities().size()) : 0);
}

QVariant CommutesListModel::data(const QModelIndex& index, int role) const
{
        auto commute = m_location->GetOutgoingCommuningCities()[index.row()];
        switch (role) {
        case Roles::Destination: return QString::fromStdString(commute.first->GetName());
        case Roles::Amount: return QString::number(commute.second);
        default: break;
        }
        return QVariant();
}

CommutesListModel::CommutesListModel(QObject* parent) : QAbstractListModel(parent), m_location(nullptr) {}

QHash<int, QByteArray> CommutesListModel::roleNames() const
{
        QHash<int, QByteArray> roles;
        roles[Roles::Destination] = "destination";
        roles[Roles::Amount]      = "amount";
        return roles;
}

int CommutesListModel::columnCount(const QModelIndex& /*parent*/) const { return 2; }

void CommutesListModel::SetCommutes(std::set<std::shared_ptr<gengeopop::Location>> locations)
{
        auto oldAmtRows = m_location == nullptr ? 0 : static_cast<int>(m_location->GetOutgoingCommuningCities().size());
        if (locations.size() != 1) {
                m_location = nullptr;
                beginRemoveRows(QModelIndex(), 0, oldAmtRows);
                endRemoveRows();
                HasCommutes(false);
                return;
        }

        m_location = *locations.begin();
        // Signal if the commutes need to be shown.
        // If there are no commutes other components may want to adapt to this
        HasCommutes(m_location->GetOutgoingCommuningCities().size() > 0);
        int newAmount = static_cast<int>(m_location->GetOutgoingCommuningCities().size());
        int diff      = newAmount - oldAmtRows;
        if (diff < 0) {
                beginRemoveRows(QModelIndex(), 0, -diff);
                endRemoveRows();
        } else if (diff > 0) {
                beginInsertRows(QModelIndex(), 0, diff - 1);
                endInsertRows();
        }
        dataChanged(createIndex(0, 0), createIndex(newAmount - 1, 2));
}
