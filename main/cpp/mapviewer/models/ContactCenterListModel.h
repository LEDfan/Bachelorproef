#pragma once

#include <QtCore/QAbstractListModel>
#include <gengeopop/ContactCenter.h>
#include <gengeopop/Location.h>
#include <set>

class ContactCenterListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        explicit ContactCenterListModel(QObject* parent = nullptr);
        QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int                    columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE
        void SetCenters(std::set<std::shared_ptr<gengeopop::Location>> locs);

        Q_INVOKABLE
        void HandleClickRow(unsigned int row);

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
