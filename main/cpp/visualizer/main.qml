import QtQuick 2.0
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtPositioning 5.6
import io.bistromatics.backend 1.0

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
				checked: true
			}

			CheckBox {
				id: checkBox_HighSchool
				text: qsTr("HighSchool")
				checked: true
			}

			CheckBox {
				id: checkBox_Workplace
				text: qsTr("Workplace")
				checked: true
			}

			CheckBox {
				id: checkBox_HouseHold
				text: qsTr("HouseHold")
				checked: true
			}
			CheckBox {
				id: checkBox_Commutes
				text: qsTr("Commutes")
				checked: true
			}

			Button {
				id: button
				text: qsTr("Open File")
                onClicked: fileSelector.open()
				checked: true
			}


		}
    }

    Window {
        width: 512
        height: 512
        visible: true

        Plugin {
            id: mapPlugin
            name: "esri"
        }

        Map {
            id: map
            anchors.fill: parent
            plugin: mapPlugin
            zoomLevel: 14
            center: QtPositioning.coordinate(51.2, 4.4)
            Component.onCompleted: backend.setObjects(map)

            function addMarker(lon, lat) {
                console.log("qml ADDING MARKER")
                var circle = Qt.createQmlObject("   import QtLocation 5.3;
                                                    import QtQuick 2.7;
                                                    MapQuickItem {
                                                        id: marker
                                                        sourceItem: Rectangle { width: 14; height: 14; color: '#e41e25'; border.width: 2; border.color: 'white'; smooth: true; radius: 7 }
                                                       coordinate {
                                                                   latitude: 51.2
                                                                   longitude: 4.4
                                                               }
                                                        opacity: 1.0

                                                        MouseArea {
                                                            anchors.fill: parent
                                                            onClicked: {parent.sourceItem.color = 'green'}
                                                        }

                                                    }", map)
                map.addMapItem(circle)
            }

            function printLogg() {
                console.log("LOGG")
            }
        }

        BackEnd {
            id: backend
        }
    }

    FileDialog {
        id: fileSelector
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileSelector.fileUrls)
            backend.loadGeoGridFromFile(fileSelector.fileUrl, map)
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
    }
}
