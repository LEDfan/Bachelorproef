/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

import QtLocation 5.3
import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import QtQuick.Window 2.0
import io.bistromatics.backend 1.0

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("GuiController")

    property var autostepping: false;

    Backend {
        id: backend
        objectName: 'backend'
        Component.onCompleted: {
            backend.Stepped.connect(stepped)
        }

        function stepped(infectedCount, day) {
            stepButton.enabled = true
            stepButtonOneDay.enabled = true
            infectedNr.text = infectedCount
            dayNr.text = day
            if (autostepping) {
                stepTimer.start();
            }
        }

    }

    ColumnLayout {
        RowLayout {
            Text {
                text: "Autostep: "
            }
            SpinBox {
                id: secondsPerDay
                decimals: 3
                maximumValue: 1000
                minimumValue: 0
                value: 2
            }

            Text {
                text: "seconds between days"
            }
            Button {
                text: "Start"
                onClicked: {
                    if (text == "Start") {
                        stepTimer.interval = secondsPerDay.value * 1000
                        stepTimer.start()
                        autostepping = true;
                        text = "Pause"
                    } else {
                        stepTimer.stop()
                        autostepping = false;
                        text = "Start"
                    }

                }
            }
            Timer {
                id: stepTimer
                running: false
                repeat: false
                onTriggered: stepDay(1)
            }
        }
        RowLayout {
            SpinBox {
                id: stepsPerClick
                decimals: 0
                minimumValue: 0
                value: 1
            }

            Button {
                id: stepButton
                text: "Multi-Step"
                onClicked: stepDay(stepsPerClick.value)
            }
            Button {
                id: stepButtonOneDay
                text: "Step"
                onClicked: stepDay(1)
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

    function stepDay(amtOfDays) {
        for(var i=0; i<amtOfDays; i++){
            stepButton.enabled = false
            stepButtonOneDay.enabled = false
            backend.StepDay()
        }
    }
}
