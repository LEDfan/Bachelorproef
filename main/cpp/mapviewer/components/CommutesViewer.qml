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