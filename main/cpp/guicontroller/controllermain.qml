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
            onClicked: backend.stepDay()
        }
    }
}
