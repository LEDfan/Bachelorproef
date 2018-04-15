import QtLocation 5.3;
import QtQuick 2.7;
MapQuickItem {
    id: marker
    sourceItem: Rectangle {id: rect; width: 20; height: 20; color: '#e41e25'; border.width: 2; border.color: 'white'; smooth: true; radius: 0; objectName: 'rect' }
    coordinate {
               latitude: 51.2
               longitude: 4.4
           }
    opacity: 0.6
    zoomLevel: 0.0
    property var markerID: ''
    property var prevColor: '#00000000'

    function setID(newID) {
        markerID = newID
    }

    signal clicked(string id, var mouse)
    signal hovered(string id)
    signal hoveredOff(string id)
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onClicked: {
            parent.clicked(markerID, mouse)
            mouse.accepted = true
        }
        onEntered: {
            parent.hovered(markerID)
        }
        onExited: {
            parent.hoveredOff(markerID)
        }
    }

}
