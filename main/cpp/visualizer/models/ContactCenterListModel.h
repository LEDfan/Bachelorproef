#pragma once

#include <QtCore/QAbstractListModel>
#include <gengeopop/ContactCenter.h>
#include <gengeopop/Location.h>

class ContactCenterListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        ContactCenterListModel(QObject* parent = 0);
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QHash<int, QByteArray> roleNames() const;

        Q_INVOKABLE
        void setCenters(std::shared_ptr<gengeopop::Location> loc);

        Q_INVOKABLE
        void handleClickRow(unsigned int row);

signals:
        void ContactCenterSelected(std::shared_ptr<gengeopop::ContactCenter> cc);

private:
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                TypeRole,
                PoolSize,
                MaxPools
        };

        std::vector<std::shared_ptr<gengeopop::ContactCenter>> m_centers;
};
