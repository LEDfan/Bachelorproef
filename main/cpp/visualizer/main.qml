import QtQuick 2.7
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtPositioning 5.5
import io.bistromatics.backend 1.0
import QtQuick.Dialogs 1.2
import "components"
import "models"

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Visualizer")

    HelpDialog{
        id: helpDialog
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                        text: "Open"
                        onTriggered: fileSelector.open()
                    }
            MenuItem {
                        text: "Save"
                        onTriggered: saveFileSelector.open()
                      }
        }

        Menu {
            title: "About"
            MenuItem {
                         text: "Help"
                         onTriggered: helpDialog.open()
                     }
        }
    }

    RowLayout {
        spacing: 6
        anchors.fill: parent
        anchors.margins: 20
        Layout.fillWidth: true


        GeoGridMap {
            id: geogridmap
        }

        LocationViewer {
            id: locViewer

            Component.onCompleted: {
                locViewer.contactCenterSelected.connect(ccViewer.showCenter)
            }

            Layout.maximumWidth: 300
        }

        ContactCenterViewer {
            Layout.maximumWidth: 200
            id: ccViewer
        }

        ColumnLayout {
            Layout.rightMargin: 4
            Layout.fillWidth: false
            MessageDialog {
                id: errorDialogBox
                objectName: 'errorDialog'
            }
        }
    }

    Backend {
        id: backend
        Component.onCompleted: {
            backend.LocationsSelected.connect(clickSignal)
        }

        function clickSignal (arg) {
            locViewer.showLocations(arg)
        }
    }

    Shortcut {
            sequence: "Ctrl+A"
            onActivated: backend.selectAll()
    }

    FileDialog {
        id: saveFileSelector
        selectExisting: false
        title: "Select a save location"
        folder: shortcuts.home
        onAccepted: {
            backend.SaveGeoGridToFile(fileUrl, errorDialogBox)
        }
    }

    FileDialog {
        id: fileSelector
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            backend.LoadGeoGridFromFile(fileSelector.fileUrl, errorDialogBox)
        }
    }
}
