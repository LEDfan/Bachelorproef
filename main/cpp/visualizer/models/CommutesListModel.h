#pragma once

#include <QtCore/QAbstractListModel>
#include <gengeopop/Location.h>

class CommutesListModel : public QAbstractListModel
{
        Q_OBJECT
public:
        CommutesListModel(QObject* parent = nullptr);
        QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int                    columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const;

        Q_INVOKABLE
        void setCommutes(std::set<std::shared_ptr<gengeopop::Location> > locations);

    signals:
    void hasCommutes(bool status);


private:
        enum Roles
        {
                IDRole = Qt::UserRole + 1,
                Destination,
                Amount
        };

        std::shared_ptr<gengeopop::Location> m_location; ///< The location we show the commutes of
};
