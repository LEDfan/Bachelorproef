#include "LocationViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void LocationViewerBackend::showLocation(std::shared_ptr<gengeopop::Location> location)
{
        QObject* nameText     = parent()->findChild<QObject*>(QString("textName"));
        QObject* provinceText = parent()->findChild<QObject*>(QString("textProvince"));
        QObject* idText       = parent()->findChild<QObject*>(QString("textID"));
        QObject* ccList       = parent()->findChild<QObject*>(QString("cclist"));
        QObject* ccListModel  = parent()->findChild<QObject*>(QString("ccListModel"));

        QString nameString     = "Location: " + QString::fromStdString(location->getName());
        QString provinceString = "Province: " + QString::number(location->getProvince());
        QString idString       = "ID: " + QString::number(location->getID());

        nameText->setProperty("text", nameString);
        provinceText->setProperty("text", provinceString);
        idText->setProperty("text", idString);

        QVariant returnVal;
        QMetaObject::invokeMethod(ccList, "clearTable", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal));

        for (auto center : location->getContactCenters()) {
                QVariantMap map;
                map.insert("type", QString::fromStdString(center->getType()));
                map.insert("maxPools", center->getMaxPools());
                map.insert("poolSize", center->getPoolSize());
                QMetaObject::invokeMethod(ccList, "addRow", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnVal),
                                          Q_ARG(QVariant, QVariant::fromValue(map)));
        }
}

LocationViewerBackend::LocationViewerBackend(QObject* parent) : QObject(parent) { _parent = parent; }
