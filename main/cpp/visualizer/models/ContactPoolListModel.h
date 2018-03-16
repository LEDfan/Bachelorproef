#pragma once

#include <QtCore/QAbstractListModel>
#include <gengeopop/ContactPool.h>
#include <gengeopop/Location.h>

class ContactPoolListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        ContactPoolListModel(QObject* parent = 0);
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QHash<int, QByteArray> roleNames() const;

        Q_INVOKABLE
        void setCenters(std::shared_ptr<gengeopop::ContactCenter> loc);

private:
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                Capacity,
                UsedCapacity

        };

        std::vector<std::shared_ptr<gengeopop::ContactPool>> m_pools;
};
