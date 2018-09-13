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

import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import "../models"
import io.bistromatics.locationviewerbackend 1.0
import io.bistromatics.contactcenterlistmodel 1.0

ColumnLayout {
    Layout.fillWidth: true
    id: locViewerBase

    signal contactCenterSelected(var cc)

    Text {
        id: textName
        objectName: 'textName'
        text: 'Location:'
        width: 100
        clip: true
                wrapMode: Text.WordWrap
    }
    Text {
        id: textPopulation
        objectName: 'textPopulation'
        text: 'Population:'
        width: 100
        clip: true
        wrapMode: Text.WordWrap
    }
    Text {
        id: textInfrected
        objectName: 'textInfected'
        text: 'Infected:'
        width: 100
        clip: true
        wrapMode: Text.WordWrap
    }
    Text {
        id: textProvince
        objectName: 'textProvince'
        text: 'Province:'
        width: 100
        clip: true
                wrapMode: Text.WordWrap
    }
    Text {
        id: textID
        objectName: 'textID'
        text: 'ID:'
        width: 100
        clip: true
        wrapMode: Text.WordWrap
    }
    Text {
        text: 'ContactCenters:'
    }
    TableView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        id: cclist
        width: parent.width
        objectName: 'cclist'
        height: 50
        model: ccModel

        TableViewColumn {
            role: "type"
            title: "Type"
            width: 200
        }

        Component.onCompleted: {
            // Bind click row to handle click of model
            cclist.clicked.connect(ccModel.HandleClickRow)
            // Bind contactCenter selected of model outside
            ccModel.ContactCenterSelected.connect(locViewerBase.contactCenterSelected)

        }
    }

    LocationViewerBackend {
        id: locationViewerBackend
    }

    function showLocations(location) {
        locationViewerBackend.ShowLocations(location)
        ccModel.SetCenters(location)
    }

    function updateInfected() {
        locationViewerBackend.UpdateInfected()
    }
}
