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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

import QtLocation 5.3;
import QtQuick 2.0;
MapQuickItem {
    id: marker
    sourceItem: Rectangle {id: rect; width: 20; height: 20; color: '#FFFFFF'; border.width: 3; border.color: 'black'; radius: 0; objectName: 'rect' }
    coordinate {
               latitude: 51.2
               longitude: 4.4
           }
    opacity: 0.85
    zoomLevel: 0.0
    property var markerID: ''
    property var regionID: ''
    property var prevColor: '#00000000'

    function setID(region, newID) {
        regionID = region
        markerID = newID
    }

    function setColor(color){
        rect.color = Qt.hsla(color, 1.0, 0.5)
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
