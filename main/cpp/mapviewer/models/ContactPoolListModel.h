#pragma once

#include <QtCore/QAbstractListModel>
#include <gengeopop/Location.h>
#include <pool/ContactPool.h>

class ContactPoolListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        ContactPoolListModel(QObject* parent = nullptr);
        QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int                    columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE
        void SetPools(std::shared_ptr<gengeopop::ContactCenter> cc);

private:
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                Capacity,
                UsedCapacity
        };

        std::vector<stride::ContactPool*> m_pools;
};
