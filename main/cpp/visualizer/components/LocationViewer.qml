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
            cclist.clicked.connect(ccModel.handleClickRow)
            // Bind contactCenter selected of model outside
            ccModel.ContactCenterSelected.connect(locViewerBase.contactCenterSelected)
        }
    }

    LocationViewerBackend {
        id: locationViewerBackend
    }

    function showLocation(location) {
        locationViewerBackend.showLocation(location)
        ccModel.setCenters(location)
    }
}