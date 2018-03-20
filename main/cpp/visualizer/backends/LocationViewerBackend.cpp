#include "LocationViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void LocationViewerBackend::showLocations(std::vector<std::shared_ptr<gengeopop::Location>> locations)
{
        QObject* nameText     = parent()->findChild<QObject*>(QString("textName"));
        QObject* provinceText = parent()->findChild<QObject*>(QString("textProvince"));
        QObject* idText       = parent()->findChild<QObject*>(QString("textID"));

        QString nameString("Name: ");
        QString provinceString("Province: ");
        QString idString("ID: ");

        if (locations.size() == 1) {
                auto location = locations[0];
                nameString += QString::fromStdString(location->getName());
                provinceString += QString::number(location->getProvince());
                idString += QString::number(location->getID());
        }

        nameText->setProperty("text", nameString);
        provinceText->setProperty("text", provinceString);
        idText->setProperty("text", idString);
}

LocationViewerBackend::LocationViewerBackend(QObject* parent) : QObject(parent) {}
