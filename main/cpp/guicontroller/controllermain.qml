import QtQuick 2.5
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("GuiController")


ColumnLayout {
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
                text: "None"
            }
            Button {
                text: "Select config"
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
    }

    ColumnLayout {
        RowLayout {
            CheckBox {

            }
            Text {
                text: "Auto Step"
            }
        }


        RowLayout {
            TextInput {
                inputMethodHints: Qt.ImhDigitsOnly
                width: 20
            }

            Text {
                text: "Seconds / Day"
            }
        }
        Button {
            text: "Step day"
        }
    }

}


}
