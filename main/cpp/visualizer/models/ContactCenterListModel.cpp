//
// Created by niels on 15.03.18.
//

#include "ContactCenterListModel.h"
#include <iostream>

int ContactCenterListModel::rowCount(const QModelIndex& parent) const
{
        std::cout << "CClistmodel row asked " << std::endl;
        return 3;
}

QVariant ContactCenterListModel::data(const QModelIndex& index, int role) const
{
        std::cout << "CClistmodel index asked " << index.row() << "," << index.column() << std::endl;
        return QString::number(role);
}

ContactCenterListModel::ContactCenterListModel(QObject* parent) : QAbstractListModel(parent)
{
        std::cout << "CClistmodel const" << std::endl;
}

QVariant ContactCenterListModel::headerData(int section, Qt::Orientation orientation, int role)
{
        std::cout << "CClistmodel header asked " << std::endl;
        return QVariant(QString("TEST"));
}

QHash<int, QByteArray> ContactCenterListModel::roleNames() const
{
        std::cout << "roles asked " << std::endl;
        QHash<int, QByteArray> roles;
        roles[Roles::IDRole]   = "id";
        roles[Roles::TypeRole] = "type";
        return roles;
}

int ContactCenterListModel::columnCount(const QModelIndex& parent) const { return 2; }
