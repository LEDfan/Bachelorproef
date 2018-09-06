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
import QtQuick.Layouts 1.2
import "../models"
import io.bistromatics.locationviewerbackend 1.0
import io.bistromatics.contactcenterlistmodel 1.0

ColumnLayout {
    Layout.fillWidth: true
    id: commutesViewerBase

    TableView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        id: commuteslist
        width: parent.width
        objectName: 'commuteslist'
        height: 50
        model: commutesModel
        visible: false

        TableViewColumn {
            role: "destination"
            title: "Destination"
            width: 200
        }
        TableViewColumn {
            role: "amount"
            title: "Amount"
            width: 100
        }

        Component.onCompleted: {
            commutesModel.HasCommutes.connect(hasCommutesChanged)
        }

        function hasCommutesChanged(status) {
            commuteslist.visible = status
            Layout.fillHeight= status
        }
    }

    function showCommutes(locations) {
        commutesModel.SetCommutes(locations)
    }
}
