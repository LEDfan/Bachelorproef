import QtLocation 5.3
import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import QtQuick.Window 2.0

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("GuiController")

    ColumnLayout {
        RowLayout {
            Text {
                text: "Autostep: "
            }
            TextInput {
                id: secondsPerDay
                inputMethodHints: Qt.ImhDigitsOnly
                width: 20
                text: "2"
            }

            Text {
                text: "Seconds / Day"
            }
            Button {
                text: "Start"
                onClicked: {
                    if (text == "Start") {
                        stepTimer.interval = secondsPerDay.text * 1000
                        stepTimer.start()
                        text = "Stop"
                    } else {
                        stepTimer.stop()
                        text = "Start"
                    }

                }
            }
            Timer {
                id: stepTimer
                running: false
                repeat: true
                onTriggered: stepDay()
            }
        }
        RowLayout {
            Button {
                text: "Step day"
                onClicked: stepDay()
            }
        }

        RowLayout {
            Text {
                text: "Current day: "
            }
            Text {
                id: dayNr
                text: "0"
            }
        }
        RowLayout {
            Text {
                text: "Infected people: "
            }
            Text {
                id: infectedNr
                text: "0"
            }
        }
    }

    function stepDay() {
        backend.StepDay()
        dayNr.text = backend.GetDay()
        infectedNr.text = backend.GetInfectedCount()
    }
}
