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
    Text {
        id: textSelCCID
        objectName: 'textCCID'
        text: 'Contact Center: 24'
    }
    Text {
        id: textSelCCMaxPoolSize
        objectName: 'textCCPoolSize'
        text: 'Max pool size: 42'
    }
    Text {
        id: textSelCCMaxPools
        objectName: 'textCCMaxPools'
        text: 'Max pools: 42'
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
            role: "ID"
            title: "ID"
            width: 50
        }
        TableViewColumn {
            role: "capacity"
            title: "Capacity"
            width: 50
        }
        TableViewColumn {
            role: "usedCapacity"
            title: "Used Capacity"
            width: 50
        }
    }

    ContactCenterViewerBackend {
        id: ccBackend
    }

    function showCenter(center) {
        cpModel.setPools(center)
        ccBackend.showContactCenter(center)
    }
}