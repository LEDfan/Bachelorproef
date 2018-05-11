import QtQuick 2.5
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.3

import io.bistromatics.launcher 1.0


ApplicationWindow {
    id: window
    visible: true
    title: qsTr("GuiController")
    Launcher {
        id: launcher
    }

    FileDialog {
        id: configSelector
        onAccepted: {
            console.warn("selected file: ", configSelector.fileUrl)
            launcher.setConfigPath(configSelector.fileUrl)
            configUrlText.text = configSelector.fileUrl
        }
    }

    ColumnLayout {
        id: mainLayout
        x: 21
        y: 22
        width: 421
        height: 217
        Layout.fillWidth: true
        // Select config
        RowLayout {
            Layout.fillHeight: false
            Layout.fillWidth: true
            Column {
                Text{
                    text: "Selected config: "
                    font.pointSize: 16
                }
            }
            Column {
                Text{
                    id: configUrlText
                    font.weight: Font.Light
                    style: Text.Normal
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Column {
                id: column
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Button {
                    text: "Select config"
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    checkable: true
                    Layout.fillWidth: false
                    onClicked: configSelector.open()
                }
            }
        }

        GroupBox {
            id: groupBox
            x: 209
            y: 50
            width: 147
            height: 210
            visible: true
            checkable: false
            checked: false
            title: "​"
            Layout.fillWidth: true
            Layout.fillHeight: true
            flat: false
            Column {
                spacing: 10

                // Viewers
                ColumnLayout {
                    Row {
                        x: 10
                        y: 10
                        spacing: 10
                        CheckBox {

                            id: adoptedViewerCheckbox
                        }
                        Text {
                            y: 0
                            text: "AdoptedViewer"
                            font.pointSize: 13
                        }
                    }
                    Row {
                        x: 10
                        y: 38
                        spacing: 10
                        CheckBox {

                            id: cliViewerCheckbox
                        }
                        Text {
                            text: "CliViewer"
                            font.pointSize: 13
                        }
                    }
                    Row {
                        x: 10
                        y: 66
                        spacing: 10
                        CheckBox {

                            id: infectedViewerCheckbox
                        }
                        Text {
                            text: "InfectedViewer"
                            font.pointSize: 13
                        }
                    }
                    Row {
                        x: 10
                        y: 94
                        spacing: 10
                        CheckBox {

                            id: mapViewerCheckbox
                        }
                        Text {
                            text: "MapViewer"
                            font.pointSize: 13
                        }
                    }
                    Row {
                        x: 10
                        y: 122
                        spacing: 10
                        CheckBox {

                            id: personsViewerCheckbox
                        }
                        Text {
                            text: "PersonsViewer"
                            font.pointSize: 13
                        }
                    }
                    Row {
                        x: 10
                        y: 150
                        spacing: 10
                        CheckBox {
                            id: summaryViewerCheckbox

                        }
                        Text {
                            text: "SummaryViewer"
                            font.pointSize: 13
                        }
                    }
                }

            }

        }
        Button {
            y: 280
            text:  "Launch!"
            transformOrigin: Item.Center
            Layout.fillWidth: true
            anchors.top: groupBox.bottom
            anchors.topMargin: 20

            onClicked: {
                console.warn("Launching...")
                launcher.launch(
                    mapViewerCheckbox.checked,
                    adoptedViewerCheckbox.checked,
                    cliViewerCheckbox.checked,
                    infectedViewerCheckbox.checked,
                    personsViewerCheckbox.checked,
                    summaryViewerCheckbox.checked
                )
            }

        }
    }
}
