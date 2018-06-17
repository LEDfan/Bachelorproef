import QtQuick 2.5
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtPositioning 5.5
import io.bistromatics.backend 1.0
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0

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
    }

    property variant mapCenter: QtPositioning.coordinate(50.84667, 4.35472)

    function fitViewport() {
        map.fitViewportToVisibleMapItems()
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        zoomLevel: 8
        center: mapCenter
        Layout.fillHeight: true
        Layout.fillWidth: true
        maximumTilt: 0

        Component.onCompleted: {
            backend.SetObjects(map)
            for( var i_type in supportedMapTypes  ) {
                if( supportedMapTypes[i_type].name.localeCompare( "Custom URL Map"  ) === 0  ) {
                activeMapType = supportedMapTypes[i_type]
                }
            }
        }

        MapRectangle {
            id: 'selectionRectangle'
            color: 'blue'
            opacity: 0.25
            border.width: 2
            topLeft {
                latitude: 0
                longitude: 0
            }
            bottomRight {
                latitude: 0
                longitude: 0
            }
        }

        MouseArea {
            anchors.fill: parent
            property var start
            property bool rectSelectStarted: false
            onPressed: {
                parent.mapClicked(mouse)
                if(mouse.modifiers & Qt.ControlModifier){
                    // Disable panning
                    map.gesture.enabled = false
                    // Set accepted so we do no propagate click
                    mouse.accepted = true
                    rectSelectStarted = true
                    // Save the start coordiate
                    start = map.toCoordinate(Qt.point(mouse.x, mouse.y), false)
                    map.addMapItem(selectionRectangle)
                }
            }
            onPositionChanged: {
                if(rectSelectStarted) {
                    // Get the end coordinate of the selection
                    var end = map.toCoordinate(Qt.point(mouse.x, mouse.y), false)

                    if(mouse.modifiers & Qt.ShiftModifier){
                        backend.SelectExtraInArea(start.latitude, start.longitude, end.latitude, end.longitude)
                    } else {
                        backend.SelectArea(start.latitude, start.longitude, end.latitude, end.longitude)
                    }
                    // Fix order
                    var tstart = start;
                    var tend = end;
                    if(end.longitude < start.longitude) {
                        tstart = end;
                        tend = start;
                    }
                    // Show it on the map
                    selectionRectangle.opacity = 0.3
                    selectionRectangle.topLeft.latitude = tstart.latitude
                    selectionRectangle.topLeft.longitude = tstart.longitude
                    selectionRectangle.bottomRight.latitude = tend.latitude
                    selectionRectangle.bottomRight.longitude = tend.longitude
                }
            }
            onReleased: {
                if(rectSelectStarted) {
                    rectSelectStarted = false
                    // Enable panning again
                    map.gesture.enabled = true
                    mouse.accepted = true
                    // Hide selection rectangle
                    selectionRectangle.opacity = 0
                    map.removeMapItem(selectionRectangle)
                }
            }
        }

        function addMarker(lat, lon, region, markerID, size, selected, specialmarker) {
            var markerComp = Qt.createComponent("qrc:/components/CustomMarker.qml")
            var marker = markerComp.createObject()
            marker.sourceItem.width =  size
            marker.sourceItem.height =  size
            marker.sourceItem.radius =  size
            if(specialmarker){
                marker.sourceItem.radius =  0
            }
            marker.anchorPoint.x = size/2
            marker.anchorPoint.y =  size/2
            marker.clicked.connect(markerClicked)
            marker.hovered.connect(markerHovered)
            marker.hoveredOff.connect(markerHoveredOff)
            marker.setID(region, markerID)
            marker.coordinate.latitude = lat
            marker.coordinate.longitude = lon
            if(selected){
                marker.sourceItem.color = 'blue'
            }
            map.addMapItem(marker)
            backend.SaveMarker(region, markerID, marker)
        }

        function addCommute(latFrom, longFrom, latTo, longTo){
            var commuteLineComp = Qt.createComponent("qrc:/components/CommuteLine.qml")
            var commuteLine = commuteLineComp.createObject()
            var startCoord = commuteLine.path[0]
            var path = commuteLine.path
            path[0].latitude = latFrom
            path[0].longitude = longFrom
            path[1].latitude = latTo
            path[1].longitude = longTo
            commuteLine.path = path
            map.addMapItem(commuteLine)
            return commuteLine;

        }

        function markerClicked(region, id, event) {
            if(event.modifiers & Qt.ControlModifier){
                backend.OnExtraMarkerClicked(region, id)
            } else {
                backend.OnMarkerClicked(region, id)
            }
        }

        function markerHovered(region, id) {
            backend.OnMarkerHovered(region, id)
        }

        function markerHoveredOff(region, id) {
            backend.OnMarkerHoveredOff(region, id)
        }

        function mapClicked(event) {
            if( ! (event.modifiers & Qt.ControlModifier) && !(event.modifiers & Qt.AltModifier)){
                event.accepted = true
                backend.ClearSelection()
                selectionRectangle.opacity = 0
            }
        }

        function clearMap() {
            // Remove the map
            map.clearMapItems()
        }
    }

}
