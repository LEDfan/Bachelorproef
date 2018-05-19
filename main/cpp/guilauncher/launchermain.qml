import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.3

import io.bistromatics.launcher 1.0


ApplicationWindow {
    id: window
    visible: true
    title: qsTr("Launcher")
    width:850
    height: 850

    FileDialog {
        id: configSelector
        onAccepted: {
            launcher.SetConfigPath(configSelector.fileUrl)
        }
    }

    ColumnLayout {
        id: mainLayout
        width: 830
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
        anchors.rightMargin: 5

        GridLayout {
            columns: 4
            rowSpacing: 5
            columnSpacing: 5

            Text {
                Layout.columnSpan: 4
                Layout.alignment: Qt.AlignCenter
                text: "Configuration"
                width: 150
                font.pointSize: 16
            }

            Button {
                text: "Open file"
                anchors.leftMargin: 20
                checkable: true
                Layout.fillWidth: true
                Layout.columnSpan: 4
                onClicked: configSelector.open()
            }

            Label { text: "Age contact matrix file" }
            TextField {
                    objectName: "inputAgeContactMatrixFile"
                    Layout.fillWidth: true
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            color: "white"
                            border.color: "#7C7C7C"
                            border.width: 1
                        }
                    }
            }

            Label { text: "Behaviour policy" }
            ComboBox {
                    objectName: "inputBehaviourPolicy"
                    Layout.fillWidth: true
                    model: ["NoBehaviour"]
            }

            Label { text: "Belief policy" }
            ComboBox {
                    objectName: "inputBeliefPolicy"
                    Layout.fillWidth: true
                    model: ["NoBelief"]
            }

            Label { text: "Contact log level" }
            ComboBox {
                    objectName: "inputContactLogLevel"
                    Layout.fillWidth: true
                    model: ["None", "All", "Transmissions", "Susceptibles"]
            }

            Label { text: "Disease Config File" }
            TextField {
                    objectName: "inputDiseaseConfigFile"
                    Layout.fillWidth: true
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            color: "white"
                            border.color: "#7C7C7C"
                            border.width: 1
                        }
                    }
            }

            Label { text: "Global Information Policy" }
            ComboBox {
                    objectName: "inputGlobalInformationPolicy"
                    Layout.fillWidth: true
                    model: ["NoGlobalInformation"]
            }

            Label { text: "Holidays file" }
            TextField {
                    objectName: "inputHolidaysFile"
                    Layout.fillWidth: true
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            color: "white"
                            border.color: "#7C7C7C"
                            border.width: 1
                        }
                    }
            }

            Label { text: "Immunity profile" }
            ComboBox {
                    objectName: "inputImmunityProfile"
                    Layout.fillWidth: true
                    model: ["None", "Random"]
            }

            Label { text: "Immunity rate" }
            SpinBox {
                    objectName: "inputImmunityRate"
                    Layout.fillWidth: true
                    minimumValue: 0
                    maximumValue: 1
                    decimals: 2
            }

            Label { text: "Local information policy" }
            ComboBox {
                    objectName: "inputLocalInformationPolicy"
                    Layout.fillWidth: true
                    model: ["LocalDiscussion", "NoLocalInformation"]
            }

            Label { text: "Number of days" }
            SpinBox {
                    objectName: "inputNumDays"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "Number of participants in survety" }
            SpinBox {
                    objectName: "inputNumberParticipantsSurvey"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "Output Adopted" }
            CheckBox {
                    objectName: "inputOutputAdopted"
            }

            Label { text: "Output Cases" }
            CheckBox {
                    objectName: "inputOutputCases"
            }

            Label { text: "Output Persons" }
            CheckBox {
                    objectName: "inputOutputPersons"
            }

            Label { text: "Output prefix" }
            TextField {
                    objectName: "inputOutputPrefix"
                    Layout.fillWidth: true
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            color: "white"
                            border.color: "#7C7C7C"
                            border.width: 1
                        }
                    }
            }

            Label { text: "Output Summary" }
            CheckBox {
                    objectName: "inputOutputSummary"
            }

            Label { text: "Number of threads" }
            SpinBox {
                    objectName: "inputNumThreads"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "Population file" }
            TextField {
                    objectName: "inputPopulationFile"
                    Layout.fillWidth: true
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            color: "white"
                            border.color: "#7C7C7C"
                            border.width: 1
                        }
                    }
            }

            Label { text: "RNG Seed" }
            SpinBox {
                    objectName: "inputRngSeed"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "RNG Type" }
            ComboBox {
                    objectName: "inputRngType"
                    Layout.fillWidth: true
                    model: ["lcg64", "lcg64_shift", "mrg2", "mrg3", "yarn2", "yarn3", ]
            }

            Label { text: "R0" }
            SpinBox {
                    objectName: "inputR0"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "Seeding age max" }
            SpinBox {
                    objectName: "inputSeedingAgeMax"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "Seeding age min" }
            SpinBox {
                    objectName: "inputSeedingAgeMin"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "Seeding rate" }
            SpinBox {
                    objectName: "inputSeedingRate"
                    minimumValue: 0
                    Layout.fillWidth: true
                    decimals: 3
            }

            Label { text: "Start date" }
            TextField {
                    objectName: "inputStartDate"
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            color: "white"
                            border.color: "#7C7C7C"
                            border.width: 1
                        }
                    }
            }

            Label { text: "Stride log level" }
            ComboBox {
                    objectName: "inputStrideLogLevel"
                    Layout.fillWidth: true
                    model: ["trace", "debug", "info", "warn", "error", "critical"]
            }

            Label { text: "Track index case" }
            CheckBox {
                    objectName: "inputTrackIndexCase"
            }

            Label { text: "Vaccine link probability" }
            SpinBox {
                    objectName: "inputVaccineLinkProbability"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Label { text: "Vaccine profile" }
            ComboBox {
                    objectName: "inputVaccineProfile"
                    Layout.fillWidth: true
                    model: ["None", "Random"]
            }

            Label { text: "Vaccine rate"}
            SpinBox {
                    objectName: "inputVaccineRate"
                    minimumValue: 0
                    Layout.fillWidth: true
            }

            Text {
                Layout.columnSpan: 4
                Layout.alignment: Qt.AlignCenter
                text: "Viewers"
                width: 150
                font.pointSize: 16
            }

            GroupBox {
                id: groupBox
                visible: true
                checkable: false
                checked: false
                title: "​"
                flat: false
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignCenter
                Column {
                    spacing: 10

                    // Viewers
                    ColumnLayout {
                        Row {
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
            Label {
                text: "Controller"
            }
            ComboBox {
                width: 200
                model: [ "CLI", "GUI"]
                onActivated: {
                    launcher.SetController(index);
                }
            }

            Button {
                text:  "Launch"
                anchors.topMargin: 20
                Layout.columnSpan: 4
                Layout.fillWidth: true

                onClicked: {
                    launcher.SetConfig(
                        mapViewerCheckbox.checked,
                        adoptedViewerCheckbox.checked,
                        cliViewerCheckbox.checked,
                        infectedViewerCheckbox.checked,
                        personsViewerCheckbox.checked,
                        summaryViewerCheckbox.checked
                    )
                    launcher.UpdatePtree();
                    window.close()
                }
            }
        }
    }
}
