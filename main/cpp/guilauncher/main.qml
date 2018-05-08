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
        anchors.margins: 2
        Layout.fillWidth: true
        // Select config
        RowLayout {
            Layout.fillWidth: true
            Text{
                text: "Selected config: "
            }
            Text{
                id: configUrlText
                text: "None"
            }
            Button {
                text: "Select config"
                onClicked: configSelector.open()
            }
        }


        // Viewers
        RowLayout {
            CheckBox {

            }
            Text {
                text: "Visualizer 1"
            }
        }
        RowLayout {
            CheckBox {

            }
            Text {
                text: "Visualizer 2"
            }
        }

        Button {
            text:  "Launch!"
            onClicked: {
                console.warn("Launching...")
                launcher.launch()
            }

        }
    }




}
