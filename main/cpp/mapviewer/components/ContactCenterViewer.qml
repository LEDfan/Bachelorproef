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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

import QtQuick 2.0
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtPositioning 5.5
import "../models"
import io.bistromatics.contactcenterviewerbackend 1.0

ColumnLayout {
    Layout.fillWidth: true
    Layout.fillHeight: true
    Text {
        id: textSelCCMaxPoolSize
        objectName: 'textCCPoolSize'
        text: 'Max pool size:'
    }
    Text {
        id: textSelCCMaxPools
        objectName: 'textCCMaxPools'
        text: 'Max pools:'
    }
    Text {
        text: 'ContactPools:'
    }
    TableView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        id: cplist
        width: parent.width
        objectName: 'cplist'
        height: 50
        model: cpModel

        TableViewColumn {
            role: "usedCapacity"
            title: "Used Capacity"
            width: 250
        }
    }

    ContactCenterViewerBackend {
        id: ccBackend
    }

    function showCenter(center) {
        cpModel.SetPools(center)
        ccBackend.ShowContactCenter(center)
    }
}
