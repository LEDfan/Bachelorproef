#include "ContactCenterViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void ContactCenterViewerBackend::showContactCenters(std::vector<std::shared_ptr<gengeopop::ContactCenter>> centers)
{
        QObject* nameText        = parent()->findChild<QObject*>(QString("textCCID"));
        QObject* poolSizeText    = parent()->findChild<QObject*>(QString("textCCPoolSize"));
        QObject* maxPoolSizeText = parent()->findChild<QObject*>(QString("textCCMaxPools"));

        QString idString("ID: ");
        QString poolsString("Pool Size: ");
        QString maxPoolsString("Max # Pools: ");

        if (centers.size() == 1) {
                auto cc = centers[0];
                idString += QString::fromStdString("0");
                poolsString += QString::number(cc->getPoolSize());
                maxPoolsString += QString::number(cc->getMaxPools());
        }

        nameText->setProperty("text", idString);
        poolSizeText->setProperty("text", poolsString);
        maxPoolSizeText->setProperty("text", maxPoolsString);
}

ContactCenterViewerBackend::ContactCenterViewerBackend(QObject* parent) : QObject(parent) {}
