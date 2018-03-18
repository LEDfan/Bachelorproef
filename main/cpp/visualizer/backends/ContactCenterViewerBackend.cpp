#include "ContactCenterViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void ContactCenterViewerBackend::showContactCenter(std::shared_ptr<gengeopop::ContactCenter> center)
{
        QObject* nameText        = parent()->findChild<QObject*>(QString("textCCID"));
        QObject* poolSizeText    = parent()->findChild<QObject*>(QString("textCCPoolSize"));
        QObject* maxPoolSizeText = parent()->findChild<QObject*>(QString("textCCMaxPools"));

        QString idString       = "ID: " + QString::fromStdString("0");
        QString poolsString    = "Pool Size: " + QString::number(center->getPoolSize());
        QString maxPoolsString = "Max # Pools: " + QString::number(center->getMaxPools());

        nameText->setProperty("text", idString);
        poolSizeText->setProperty("text", poolsString);
        maxPoolSizeText->setProperty("text", maxPoolsString);
}

ContactCenterViewerBackend::ContactCenterViewerBackend(QObject* parent) : QObject(parent) {}
