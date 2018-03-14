import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import "../models"

ColumnLayout {
    Layout.fillWidth: true
    Text {
        id: textName
        text: 'Location: Deurne-Noord'
    }
    Text {
        id: textProvince
        text: 'Province: Antwerpen'
    }
    Text {
        id: textID
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
        height: 50
        model: ContactCenterModel {}
        focus: true
        TableViewColumn {
            role: "ID"
            title: "ID"
            width: 50
        }
        TableViewColumn {
            role: "type"
            title: "Type"
            width: 100
        }


    }
}