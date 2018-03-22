#include "LocationViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void LocationViewerBackend::showLocations(std::set<std::shared_ptr<gengeopop::Location>> locations)
{
        QObject* nameText     = parent()->findChild<QObject*>(QString("textName"));
        QObject* provinceText = parent()->findChild<QObject*>(QString("textProvince"));
        QObject* idText       = parent()->findChild<QObject*>(QString("textID"));

        QString nameString("Name: ");
        QString provinceString("Province: ");
        QString idString("ID: ");

        bool first = true;
        for (auto location : locations) {
                if (first) {
                        first = false;
                } else {
                        nameString += QString(",");
                        provinceString += QString(",");
                        idString += QString(",");
                }
                nameString += QString::fromStdString(location->getName());
                provinceString += QString::number(location->getProvince());
                idString += QString::number(location->getID());
        }

        nameText->setProperty("text", nameString);
        provinceText->setProperty("text", provinceString);
        idText->setProperty("text", idString);
}

LocationViewerBackend::LocationViewerBackend(QObject* parent) : QObject(parent) {}
