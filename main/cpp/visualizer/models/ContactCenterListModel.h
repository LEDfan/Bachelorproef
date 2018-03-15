#pragma once

#include <QtCore/QAbstractListModel>

class ContactCenterListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        ContactCenterListModel(QObject* parent = 0);
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole);
        QHash<int, QByteArray> roleNames() const;

private:
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                TypeRole
        };
};
