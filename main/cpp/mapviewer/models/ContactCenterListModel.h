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

        /**
         * Returns the data at the given index and the given role.
         * @param index The row we want data of
         * @param role The role/column name we want the data of
         */
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        /**
         * Returns the amount of rows, this is the amount of
         */
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        /**
         * Returns the amount of columns, this is the amount of roles.
         */
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        /*
         * Returns the available roles, the columns.
         */
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE
        /**
         * Sets the centers we want to show.
         * @param locs The locations we want to show the contactCenters of.
         */
        void SetCenters(std::set<std::shared_ptr<gengeopop::Location>> locs);

        Q_INVOKABLE
        /**
         * This function should be invoked when a row is to be clicked. It will then get the data necessary and emit a
         * "ContactCenterSelected" signal with the corresponding ContactCenter.
         */
        void HandleClickRow(unsigned int row);

signals:
        /**
         * This signal is emmited when a Contact Center is selected. This will be after a HandleClickRow is called.
         */
        void ContactCenterSelected(std::shared_ptr<gengeopop::ContactCenter> cc);

private:
        // The roles, This can be understood as the names of the (possible) collumns.
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                TypeRole,
                PoolSize,
                MaxPools
        };

        // The ContactCenters we are displaying
        std::vector<std::shared_ptr<gengeopop::ContactCenter>> m_centers;
};
