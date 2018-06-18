import QtLocation 5.3;
import QtQuick 2.0;
import QtQuick.Dialogs 1.2

Dialog {
    visible: false
    title: "Help"

     contentItem: Text {
        text: "Open a file using the context menu.\n
                Select a single marker: Click on the marker
                Select extra marker: Hold Control and press the marker.
                Select Area: Hold control and drag over the area you want to select.
                Select Extra Area: Hold Ctrl+Shift and drag of the area.

                Pan the map without deselecting: Hold Alt while panning.

                Select All: Ctrl + A

                Show commutes: Click checkbox 'View > Show Commutes' then select cities.
                The commutes between them will be shown

                Color coding:
                Blue = No infected people at this location.
                When the infected rate goes up it will first start turning green then yellow and finally red.

                Ctrl+F will fit the viewport of the map.
                "
        anchors.centerIn: parent
    }
}
