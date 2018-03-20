import QtQuick 2.0
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtPositioning 5.5
import io.bistromatics.backend 1.0
import QtQuick.Dialogs 1.2
import "components"
import "models"

ApplicationWindow {
	id: window
	visible: true
	width: 640
	height: 480
	title: qsTr("Visualizer")

	RowLayout {
		spacing: 6
		anchors.fill: parent
        anchors.margins: 20
        Layout.fillWidth: true


        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            width: 640
            Plugin {
                id: mapPlugin
                name: "osm"
                PluginParameter { name: "osm.mapping.host"; value: "https://tile.openstreetmap.org/" }
                PluginParameter { name: "osm.geocoding.host"; value: "https://nominatim.openstreetmap.org" }
                PluginParameter { name: "osm.routing.host"; value: "https://router.project-osrm.org/viaroute" }
                PluginParameter { name: "osm.places.host"; value: "https://nominatim.openstreetmap.org/search" }
                /*PluginParameter { name: "osm.mapping.copyright"; value: "" }*/
                /*PluginParameter { name: "osm.mapping.highdpi_tiles"; value: true }*/
            }

            Map {
                id: map
                anchors.fill: parent
                plugin: mapPlugin
                zoomLevel: 14
                center: QtPositioning.coordinate(51.2, 4.4)
                Layout.fillHeight: true
                Layout.fillWidth: true

                Component.onCompleted: {
                    backend.SetObjects(map)
                    for( var i_type in supportedMapTypes  ) {
                        if( supportedMapTypes[i_type].name.localeCompare( "Custom URL Map"  ) === 0  ) {
                        activeMapType = supportedMapTypes[i_type]
                        map.addMapItem(selectionRectangle)

                        }
                    }
                }

                MapRectangle {
                    id: 'selectionRectangle'
                    color: 'blue'
                    opacity: 0.25
                    border.width: 2
                    topLeft {
                        latitude: -27
                        longitude: 153
                    }
                    bottomRight {
                        latitude: -28
                        longitude: 153.5
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    property var start
                    property bool rectSelectStarted: false
                    onPressed: {
                        parent.mapClicked(mouse)
                        console.warn("down")
                        if(mouse.modifiers & Qt.ControlModifier){
                            // Disable panning
                            map.gesture.enabled = false
                            // Set accepted so we do no propagate click
                            mouse.accepted = true
                            rectSelectStarted = true
                            // Save the start coordiate
                            start = map.toCoordinate(Qt.point(mouse.x, mouse.y), false)
                        }
                    }
                    onReleased: {
                        if(rectSelectStarted) {
                            rectSelectStarted = false
                            console.warn("up")
                            // Enable panning again
                            map.gesture.enabled = true
                            mouse.accepted = true
                            // Get the end coordinate of the selection
                            var end = map.toCoordinate(Qt.point(mouse.x, mouse.y), false)
                            backend.selectArea(start.latitude, start.longitude, end.latitude, end.longitude)
                            // Fix order
                            if(end.longitude < start.longitude) {
                                var temp = start;
                                start = end;
                                end = temp;
                            }
                            // Show it on the map
                            selectionRectangle.opacity = 0.3
                            selectionRectangle.topLeft.latitude = start.latitude
                            selectionRectangle.topLeft.longitude = start.longitude
                            selectionRectangle.bottomRight.latitude = end.latitude
                            selectionRectangle.bottomRight.longitude = end.longitude

                        }
                    }
                }

                function addMarker(lon, lat, markerID, size) {
                    var markerComp = Qt.createComponent("qrc:/components/CustomMarker.qml")
                    var marker = markerComp.createObject()
                    marker.sourceItem.width =  size
                    marker.sourceItem.height =  size
                    marker.sourceItem.radius =  size
                    marker.anchorPoint.x = size/2
                    marker.anchorPoint.y =  size/2
                    marker.clicked.connect(markerClicked)
                    marker.setID(markerID)
                    marker.coordinate.latitude = lat
                    marker.coordinate.longitude = lon
                    map.addMapItem(marker)
                }

                function markerClicked(id, event) {
                    if(event.modifiers & Qt.ShiftModifier){
                        backend.OnExtraMarkerClicked(id)
                    } else {
                        backend.OnMarkerClicked(id)
                    }
                }

                function mapClicked(event) {
                    if( ! (event.modifiers & Qt.ShiftModifier)){
                        backend.clearSelection()
                        selectionRectangle.opacity = 0
                        console.warn("Map clicked")
                    }
                }

                function clearMap() {
                    // Remove the map
                    map.clearMapItems()
                    // Re add the selection rectangle
                    map.addMapItem(selectionRectangle)
                }

            }
        }

        // LEFT COLUMN
        LocationViewer {
            id: locViewer

            Component.onCompleted: {
                locViewer.contactCenterSelected.connect(ccViewer.showCenter)
            }
        }

        // MIDDLE COLUMN Contact Center info
        ContactCenterViewer {
            id: ccViewer
        }

		ColumnLayout {
			Layout.rightMargin: 4
            Layout.fillWidth: false
            /*
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
			*/

			Button {
				id: buttonOpen
				text: qsTr("Open File")
                onClicked: fileSelector.open()
				checked: true
			}
			Button {
				id: buttonSave
				text: qsTr("Save to File")
                onClicked: saveFileSelector.open()
				checked: true
			}
			MessageDialog {
			    id: errorDialogBox
			    objectName: 'errorDialog'
			}
		}
    }

    Backend {
        id: backend
        Component.onCompleted: {
            backend.LocationsSelected.connect(clickSignal)
        }

        function clickSignal (arg) {
            locViewer.showLocations(arg)
        }
    }

    FileDialog {
        id: saveFileSelector
        selectExisting: false
        title: "Select a save location"
        folder: shortcuts.home
        onAccepted: {
            backend.SaveGeoGridToFile(fileUrl, errorDialogBox)
        }
    }

    FileDialog {
        id: fileSelector
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            backend.LoadGeoGridFromFile(fileSelector.fileUrl, errorDialogBox)
        }
    }
}