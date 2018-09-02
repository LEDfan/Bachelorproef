import QtLocation 5.3;
import QtQuick 2.0;
import QtQuick.Dialogs 1.2

Dialog {
    visible: false
    title: "Help"

     contentItem: Text {
        text: "
                To open a file use the 'File -> Open' menu.\n

                Cities are represented by circular markers.

                Click on a marker to select it.
                Hold the Ctrl key while clicking on markers to select multiple.
                Hold the Ctrl key while dragging to select all the cities in a rectangle.
                Hold the Ctrl and Shift key while dragging to select an additional rectangle.

                Markers can be deselected by clicking on the map.
                Hold the Alt key while panning the map to prevent deselection.

                Ctrl + A can be used to select all cities.
                Ctrl + F can be used to fit the viewport of the map so all cities are visible.

                To show commutes of a city, first enable the 'View -> Show Commutes'.
                The commutes between the selected cities will be shown.

                While simulating the color a blue markers indicated that no one is infected at that location.
                As soon as some people get infected the marker will become green, to eventually become red
                when more people are infected.
                "
        anchors.centerIn: parent
    }
}
