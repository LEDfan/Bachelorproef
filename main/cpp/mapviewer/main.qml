import QtQuick 2.5
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
    title: qsTr("MapViewer")

    statusBar: StatusBar {
        Layout.fillWidth: true
        RowLayout {
            Label {
                id: statusLabel
                text: ""
            }
        }
    }
    Timer {
        id: statusTimer
        repeat: false
        interval: 0
        onTriggered: {
            statusLabel.text = ""
        }
    }
    function displayStatusText(text, duration) {
        statusLabel.text = text
        statusTimer.interval = duration
        statusTimer.start()
    }
    Component.onCompleted: {
        window.showMaximized()
    }

    HelpDialog{
        id: helpDialog
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                        text: "&Open"
                        onTriggered: fileSelector.open()
                        shortcut: "Ctrl+o"
                    }
            MenuItem {
                        text: "&Save"
                        onTriggered: saveFileSelector.open()
                        shortcut: "Ctrl+s"
                      }
            MenuItem {
                        text: "&Export to PNG"
                        onTriggered: savePNGSelector.open()
                        shortcut: "Shift+Ctrl+E"
                      }
        }

        Menu {
            title: "View"
            MenuItem {
                         text: "Show &Commutes"
                         onTriggered: backend.SetShowCommutes(checked)
                         checkable: true
                         checked: false
                         shortcut: "Alt+c"
                     }
        }

        Menu {
            title: "Help"
            MenuItem {
                         text: "Shortcuts"
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


        ColumnLayout {
            Layout.maximumWidth: 320

            LocationViewer {
                id: locViewer

                Component.onCompleted: {
                    locViewer.contactCenterSelected.connect(ccViewer.showCenter)
                }

            }

            CommutesViewer {
                id: commutesViewer
            }

        }


        ContactCenterViewer {
            Layout.maximumWidth: 250
            id: ccViewer
        }

        MessageDialog {
            id: errorDialogBox
            objectName: 'errorDialog'
        }
    }
    Backend {
        id: backend
        objectName: 'backend'
        Component.onCompleted: {
            backend.LocationsSelected.connect(clickSignal)
            if (Qt.application.arguments.length > 1) {
                backend.LoadGeoGridFromCommandLine(Qt.application.arguments);
            }
        }

        function clickSignal (arg) {
            locViewer.showLocations(arg)
            commutesViewer.showCommutes(arg)
        }

    }
    Shortcut {
            sequence: "Ctrl+A"
            onActivated: backend.SelectAll()
    }

    FileDialog {
        id: savePNGSelector
        selectExisting: false
        nameFilters: ["Image files (*.jpg *.png)"]
        title: "Select a save location"
        onAccepted: {
            geogridmap.grabToImage(function(result) {
                var filename = savePNGSelector.fileUrl.toString().substring(7)
                if (!result.saveToFile(filename)) {
                    filename += ".png"
                    result.saveToFile(filename);
                }
                displayStatusText("Image exported to " + filename, 5000)
            })
        }
    }

    FileDialog {
        id: saveFileSelector
        selectExisting: false
        title: "Select a save location"
        selectFolder: true
        onAccepted: {
            backend.SaveGeoGridToFile(fileUrl, errorDialogBox)
            displayStatusText("GeoGrid saved to " + fileUrl.toString().substring(7), 5000)
        }
    }

    FileDialog {
        id: fileSelector
        title: "Please choose a file"
        onAccepted: {
            backend.LoadGeoGridFromFile(fileSelector.fileUrl, errorDialogBox)
            displayStatusText("Finished loading file " + fileSelector.fileUrl.toString().substring(7), 5000)
        }
    }
}