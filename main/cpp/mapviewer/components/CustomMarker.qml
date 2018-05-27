import QtLocation 5.3;
import QtQuick 2.0;
MapQuickItem {
    id: marker
    sourceItem: Rectangle {id: rect; width: 20; height: 20; color: '#FFFFFF'; border.width: 2; border.color: 'black'; smooth: true; radius: 0; objectName: 'rect' }
    coordinate {
               latitude: 51.2
               longitude: 4.4
           }
    opacity: 0.6
    zoomLevel: 0.0
    property var markerID: ''
    property var regionID: ''
    property var prevColor: '#00000000'

    function setID(region, newID) {
        regionID = region
        markerID = newID
    }

    function setColor(color){
        rect.color = color
    }

    function setBorder(color){
        rect.border.color = color
    }

    signal clicked(var region, string id, var mouse)
    signal hovered(var region, string id)
    signal hoveredOff(var region, string id)
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onClicked: {
            console.warn("Clicked", regionID, markerID, mouse)
            parent.clicked(regionID, markerID, mouse)
            mouse.accepted = true
        }
        onEntered: {
            parent.hovered(regionID, markerID)
        }
        onExited: {
            parent.hoveredOff(regionID, markerID)
        }
    }

}
