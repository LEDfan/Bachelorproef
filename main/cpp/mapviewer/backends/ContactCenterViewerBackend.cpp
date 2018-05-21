#include "ContactCenterViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <iostream>

void ContactCenterViewerBackend::ShowContactCenter(std::shared_ptr<gengeopop::ContactCenter> center)
{
        auto  poolSizeText    = parent()->findChild<QObject*>(QString("textCCPoolSize"));
        auto* maxPoolSizeText = parent()->findChild<QObject*>(QString("textCCMaxPools"));

        QString poolsString    = "Pool Size: " + QString::number(center->GetPoolSize());
        QString maxPoolsString = "Max # Pools: " + QString::number(center->GetMaxPools());

        poolSizeText->setProperty("text", poolsString);
        maxPoolSizeText->setProperty("text", maxPoolsString);
}

ContactCenterViewerBackend::ContactCenterViewerBackend(QObject* parent) : QObject(parent) {}
