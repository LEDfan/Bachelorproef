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
            width: 100
        }
        TableViewColumn {
            role: "poolSize"
            title: "Pool Size"
            width: 100
        }
        TableViewColumn {
            role: "maxPools"
            title: "Max # Pools"
            width: 100
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