#include "ContactCenterViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void ContactCenterViewerBackend::showContactCenter(std::shared_ptr<gengeopop::ContactCenter> cc)
{
        QObject* nameText        = parent()->findChild<QObject*>(QString("textCCID"));
        QObject* poolSizeText    = parent()->findChild<QObject*>(QString("textCCPoolSize"));
        QObject* maxPoolSizeText = parent()->findChild<QObject*>(QString("textCCMaxPools"));

        QString idString       = "ID: " + QString::fromStdString("0");
        QString poolsString    = "Pool Size: " + QString::number(cc->getPoolSize());
        QString maxPoolsString = "Max # Pools: " + QString::number(cc->getMaxPools());

        nameText->setProperty("text", idString);
        poolSizeText->setProperty("text", poolsString);
        maxPoolSizeText->setProperty("text", maxPoolsString);
}

ContactCenterViewerBackend::ContactCenterViewerBackend(QObject* parent) : QObject(parent) { _parent = parent; }
