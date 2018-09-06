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

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

import io.bistromatics.launcher 1.0


ApplicationWindow {
    id: window
    visible: true
    title: qsTr("Launcher")
    width: 820
    height: 850

    FileDialog {
        id: configSelector
        onAccepted: {
            launcher.SetConfigPath(configSelector.fileUrl)
        }
    }
    FileDialog {
        id: saveSelector
        selectExisting: false
        onAccepted: {
            launcher.SaveConfig(saveSelector.fileUrl)
        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
        ColumnLayout {
            id: mainLayout
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.left: parent.left

            Text {
                id: strideTitle
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignCenter
                text: "Stride"
                width: 150
                font.pointSize: 30
            }

            Button {
                text:  "Launch"
                anchors.top: strideTitle.bottom
                anchors.margins: 20
                Layout.columnSpan: 2
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
                    launcher.SetToLaunch();
                    window.close()
                }
            }
            GridLayout {
                anchors.top: parent.top
                anchors.topMargin: 110
                Layout.fillWidth: false
                columns: 2
                rowSpacing: 10
                columnSpacing: 20

                GridLayout {
                    columns: 2
                    rowSpacing: 10
                    columnSpacing: 20


                    Text {
                        Layout.columnSpan: 2
                        Layout.alignment: Qt.AlignCenter
                        text: "Configuration"
                        width: 150
                        font.pointSize: 16
                    }

                    Text {
                        text: "​"
                    }

                    Button {
                        text: "Open file"
                        anchors.leftMargin: 20
                        checkable: true
                        Layout.fillWidth: true
                        Layout.columnSpan: 2
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
                        maximumValue: 18446744073709551615 //ULL max
                        Layout.fillWidth: true
                    }

                    Label { text: "Number of participants in survey" }
                    SpinBox {
                        objectName: "inputNumberParticipantsSurvey"
                        minimumValue: 0
                        maximumValue: 18446744073709551615 //ULL max
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
                        maximumValue: 18446744073709551615 //ULL max
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
                        maximumValue: 18446744073709551615 //ULL max
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
                        maximumValue: 18446744073709551615 //ULL max
                        Layout.fillWidth: true
                    }

                    Label { text: "Seeding age max" }
                    SpinBox {
                        objectName: "inputSeedingAgeMax"
                        minimumValue: 0
                        Layout.fillWidth: true
                        maximumValue: 18446744073709551615 //ULL max
                    }

                    Label { text: "Seeding age min" }
                    SpinBox {
                        objectName: "inputSeedingAgeMin"
                        maximumValue: 18446744073709551615 //ULL max
                        minimumValue: 0
                        Layout.fillWidth: true
                    }

                    Label { text: "Seeding rate" }
                    SpinBox {
                        objectName: "inputSeedingRate"
                        minimumValue: 0
                        maximumValue: 18446744073709551615 //ULL max
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
                        maximumValue: 1
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
                        maximumValue: 18446744073709551615 //ULL max
                        minimumValue: 0
                        Layout.fillWidth: true
                    }
                    Label { text: "Gengeopop Type"}
                    ComboBox {
                        objectName: "gengeopopType"
                        Layout.fillWidth: true
                        model: ["default", "generate", "import", "multi-region"]
                        onCurrentIndexChanged: {
                            if (currentIndex == 0 || currentIndex == 3) {
                                window.width = 820
                                setGenGeopPopFileVisibility(false)
                                setGenGeopPopGenerateVisibility(false)
                                if (currentIndex == 0) {
                                    mapViewerCheckbox.enabled = false
                                    mapViewerCheckbox.checked = false
                                } else {
                                    mapViewerCheckbox.enabled = true
                                    mapViewerCheckbox.checked = true
                                }
                            } else if (currentIndex == 1) {
                                mapViewerCheckbox.checked = true
                                mapViewerCheckbox.enabled = true
                                window.width = 910
                                setGenGeopPopFileVisibility(false)
                                setGenGeopPopGenerateVisibility(true)
                            } else if (currentIndex == 2){
                                mapViewerCheckbox.checked = true
                                mapViewerCheckbox.enabled = true
                                window.width = 820
                                setGenGeopPopGenerateVisibility(false)
                                setGenGeopPopFileVisibility(true)
                            }
                        }
                        function setGenGeopPopGenerateVisibility(visibility) {
                            gengeopopPopulationSizeTitle.visible = visibility;
                            gengeopopPopulationSize.visible = visibility;
                            gengeopopFraction1826StudentsTitle.visible = visibility;
                            gengeopopFraction1826Students.visible = visibility;
                            gengeopopFractionActiveCommutingTitle.visible = visibility;
                            gengeopopFractionActiveCommuting.visible = visibility;
                            gengeopopFractionStudentsCommutingTitle.visible = visibility;
                            gengeopopFractionStudentsCommuting.visible = visibility;
                            gengeopopFraction1865ActiveTitle.visible = visibility;
                            gengeopopFraction1865Active.visible = visibility;
                            gengeopopCitiesFileTitle.visible = visibility;
                            gengeopopCommutingFileTitle.visible = visibility;
                            gengeopopHouseholdFileTitle.visible = visibility;
                            gengeopopCitiesFile.visible = visibility;
                            gengeopopCommutingFile.visible = visibility;
                            gengeopopHouseholdFile.visible = visibility;
                        }

                        function setGenGeopPopFileVisibility (visibility) {
                            gengeopopFileTitle.visible = visibility;
                            gengeopopFile.visible = visibility;
                        }
                    }
                    // Begin Selected Import
                    Label {
                        text: "Gengeopop File"
                        id: gengeopopFileTitle
                    }
                    TextField {
                        id: gengeopopFile
                        Layout.fillWidth: true
                        objectName: "gengeopopFile"
                        style: TextFieldStyle {
                            textColor: "black"
                            background: Rectangle {
                                color: "white"
                                border.color: "#7C7C7C"
                                border.width: 1
                            }
                        }
                    }
                    // Begin Selected Generate
                    Label {
                        text: "Population Size"
                        id: gengeopopPopulationSizeTitle
                    }
                    SpinBox {
                        id: gengeopopPopulationSize
                        objectName: "gengeopopPopulationSize"
                        minimumValue: 0
                        maximumValue: 18446744073709551615 //ULL max
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "Fraction of 18-26 years which are students"
                        id: gengeopopFraction1826StudentsTitle
                    }
                    SpinBox {
                        id: gengeopopFraction1826Students
                        objectName: "gengeopopFraction1826Students"
                        minimumValue: 0
                        maximumValue: 1
                        decimals: 3
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "Fraction of active people who are commuting"
                        id: gengeopopFractionActiveCommutingTitle
                    }
                    SpinBox {
                        id: gengeopopFractionActiveCommuting
                        objectName: "gengeopopFractionActiveCommuting"
                        maximumValue: 1
                        minimumValue: 0
                        decimals: 3
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "Fraction of students who are commuting"
                        id: gengeopopFractionStudentsCommutingTitle
                    }
                    SpinBox {
                        id: gengeopopFractionStudentsCommuting
                        objectName: "gengeopopFractionStudentsCommuting"
                        decimals: 3
                        maximumValue: 1
                        minimumValue: 0
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "Fraction of 18-65 year olds who are active"
                        id: gengeopopFraction1865ActiveTitle
                    }
                    SpinBox {
                        id: gengeopopFraction1865Active
                        objectName: "gengeopopFraction1856Active"
                        decimals: 3
                        maximumValue: 1
                        minimumValue: 0
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "Cities File"
                        id: gengeopopCitiesFileTitle
                    }
                    TextField {
                        id: gengeopopCitiesFile
                        Layout.fillWidth: true
                        objectName: "gengeopopCitiesFile"
                        style: TextFieldStyle {
                            textColor: "black"
                            background: Rectangle {
                                color: "white"
                                border.color: "#7C7C7C"
                                border.width: 1
                            }
                        }
                    }
                    Label {
                        text: "Commuting File"
                        id: gengeopopCommutingFileTitle
                    }
                    TextField {
                        id: gengeopopCommutingFile
                        Layout.fillWidth: true
                        objectName: "gengeopopCommutingFile"
                        style: TextFieldStyle {
                            textColor: "black"
                            background: Rectangle {
                                color: "white"
                                border.color: "#7C7C7C"
                                border.width: 1
                            }
                        }
                    }
                    Label {
                        text: "Household File"
                        id: gengeopopHouseholdFileTitle
                    }
                    TextField {
                        id: gengeopopHouseholdFile
                        Layout.fillWidth: true
                        objectName: "gengeopopHouseholdFile"
                        style: TextFieldStyle {
                            textColor: "black"
                            background: Rectangle {
                                color: "white"
                                border.color: "#7C7C7C"
                                border.width: 1
                            }
                        }
                    }
                    Button {
                        text: "Save changes"
                        anchors.leftMargin: 20
                        Layout.fillWidth: true
                        Layout.columnSpan: 2
                        onClicked: saveSelector.open()
                    }
                    Text {
                        text: "​"
                        font.pointSize: 20
                    }
                }

                GridLayout {
                    anchors.top: parent.top
                    columns: 2
                    rowSpacing: 10
                    columnSpacing: 20

                    Text {
                        Layout.columnSpan: 2
                        Layout.alignment: Qt.AlignCenter
                        text: "Viewers"
                        width: 150
                        font.pointSize: 16
                    }

                    GroupBox {
                        id: groupBox
                        Layout.fillWidth: true
                        visible: true
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
                                        checked: true
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
                                        checked: true
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
                                        checked: true
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
                                        checked: true
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
                                        checked: true
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
                                        checked: true
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
                        text: "Please select a Controller"
                    }
                    ComboBox {
                        width: 200
                        model: [ "GUI", "CLI"]
                        onActivated: {
                            launcher.SetController(index);
                        }
                    }

                }
            }
        }
    }
}
