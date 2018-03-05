import QtQuick 2.0
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2

ApplicationWindow {
	id: window
	visible: true
	width: 640
	height: 480
	title: qsTr("Visualizer")

	RowLayout {
		spacing: 6
		anchors.fill: parent
		TextArea {
			width: 240
			Layout.fillWidth: true
			Layout.fillHeight: true
			text:
			"Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
		}

		ColumnLayout {
			Layout.rightMargin: 4
			CheckBox {
				id: checkBox_School
				text: qsTr("School")
			}

			CheckBox {
				id: checkBox_HighSchool
				text: qsTr("HighSchool")
			}

			CheckBox {
				id: checkBox_Workplace
				text: qsTr("Workplace")
			}

			CheckBox {
				id: checkBox_HouseHold
				text: qsTr("HouseHold")
			}
			CheckBox {
				id: checkBox_Commutes
				text: qsTr("Commutes")
			}

			Button {
				id: button
				text: qsTr("Open File")
			}


		}}

		Window {
			width: 512
			height: 512
			visible: true

			Plugin {
				id: mapPlugin
				name: "esri" // "mapboxgl", "esri", ...
				// specify plugin parameters if necessary
				// PluginParameter {
				//     name:
				//     value:
				// }
			}

			Map {
				anchors.fill: parent
				plugin: mapPlugin
				center: QtPositioning.coordinate(59.91, 10.75) // Oslo
				zoomLevel: 14
			}
		}

	}
