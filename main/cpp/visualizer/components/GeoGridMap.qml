import QtQuick 2.7
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtPositioning 5.5
import io.bistromatics.backend 1.0
import QtQuick.Dialogs 1.2

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
            onPositionChanged: {
                if(rectSelectStarted) {
                    // Get the end coordinate of the selection
                    var end = map.toCoordinate(Qt.point(mouse.x, mouse.y), false)
                    backend.selectArea(start.latitude, start.longitude, end.latitude, end.longitude)
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
                }
            }
        }

        function addMarker(lat, lon, markerID, size, selected) {
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
            if(selected){
                marker.sourceItem.color = 'blue'
            }
            map.addMapItem(marker)
            return marker;
        }

        function addCommute(latFrom, longFrom, latTo, longTo){
            var commuteLine = Qt.createQmlObject('import QtQuick 2.0;
                    import QtLocation 5.3;
                    MapPolyline {
                        line.color: "#60002200"
                        line.width: 1.0
                        path: [
                            { latitude: 0, longitude: 0  },
                            { latitude: 0, longitude: 0  }
                        ]
                    }
               ',
               map,
               "id");
            var startCoord = commuteLine.path[0]
            var path = commuteLine.path
            path[0].latitude = latFrom
            path[0].longitude = longFrom
            path[1].latitude = latTo
            path[1].longitude = longTo
            commuteLine.path = path
            map.addMapItem(commuteLine)

        }

        function markerClicked(id, event) {
            if(event.modifiers & Qt.ControlModifier){
                backend.OnExtraMarkerClicked(id)
            } else {
                backend.OnMarkerClicked(id)
            }
        }

        function mapClicked(event) {
            if( ! (event.modifiers & Qt.ControlModifier)){
                backend.ClearSelectionAndRender()
                selectionRectangle.opacity = 0
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