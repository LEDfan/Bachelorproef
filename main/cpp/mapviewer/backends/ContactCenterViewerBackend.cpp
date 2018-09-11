/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#include "ContactCenterViewerBackend.h"
#include <QTableView>
#include <QtCore/QVariant>
#include <gengeopop/ContactCenter.h>
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
