import QtLocation 5.3;
import QtQuick 2.0;
MapQuickItem {
    id: marker
    sourceItem: Rectangle { width: 20; height: 20; color: '#e41e25'; border.width: 2; border.color: 'white'; smooth: true; radius: 10; objectName: 'rect' }
   coordinate {
               latitude: 51.2
               longitude: 4.4
           }
    opacity: 0.6
    zoomLevel: 0.0
    property var markerID: ''

    function setID(newID) {
        markerID = newID
    }
    signal clicked(string id, var mouse)
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            parent.clicked(markerID, mouse)
        }
    }

}
