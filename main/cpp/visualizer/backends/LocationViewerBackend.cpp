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
}

LocationViewerBackend::LocationViewerBackend(QObject* parent) : QObject(parent) { _parent = parent; }
