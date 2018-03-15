import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import "../models"
import io.bistromatics.locationviewerbackend 1.0
import io.bistromatics.contactcenterlistmodel 1.0

ColumnLayout {
    Layout.fillWidth: true
    Text {
        id: textName
        objectName: 'textName'
        text: 'Location: Deurne-Noord'
    }
    Text {
        id: textProvince
        objectName: 'textProvince'
        text: 'Province: Antwerpen'
    }
    Text {
        id: textID
        objectName: 'textID'
        text: 'ID: 42'
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
            role: "id"
            title: "ID"
            width: 50
        }
        TableViewColumn {
            role: "type"
            title: "Type"
            width: 100
        }
    }

    LocationViewerBackend {
        id: locationViewerBackend
    }

    function showLocation(location) {
        console.warn("LOCVWR QML")
        locationViewerBackend.showLocation(location)
    }
}