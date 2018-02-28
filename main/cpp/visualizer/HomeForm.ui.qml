import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.2
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Window 2.0

Page {
    width: 600
    height: 400

    title: qsTr("GeoGrid Visualizer")

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            // Send the file to the parser
            this.quit()
        }
        onRejected: {
            this.quit()
        }
    }

    Button {
        id: button
        x: 485
        y: 347
        text: qsTr("Load")
        onClicked: fileDialog.open()
    }

    Window {
        width: 512
        height: 512
        visible: true

        Plugin {
            id: mapPlugin
            name: "osm"
        }

        Map {
            anchors.fill: parent
            plugin: mapPlugin
            center: QtPositioning.coordinate(51.1804388, 4.4132008) // Oslo
            zoomLevel: 14
        }
    }
}
