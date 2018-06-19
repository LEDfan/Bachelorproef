#pragma once

#include <QtCore/QAbstractListModel>
#include <gengeopop/Location.h>
#include <pool/ContactPool.h>

/*
 * A Qt list model that implements data fetching and -setting to display a list of contactpools.
 */
class ContactPoolListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        ContactPoolListModel(QObject* parent = nullptr);

        /**
         * @param index the index we want to get the data of.
         * @param role the role we want the data of
         */
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        // Returns the amount of row, the amount of contactpools
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        // Returns the amount of columns
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        // Returns the available columns of the model
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE
        /**
         * Sets the contactpool we want to show in the model.
         * @param cc the contactecenters we want to show the ContactPools of.
         */
        void SetPools(std::shared_ptr<gengeopop::ContactCenter> cc);

private:
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                UsedCapacity
        };

        std::vector<stride::ContactPool*> m_pools;
};
