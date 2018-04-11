import QtQuick 2.0;
import QtLocation 5.3;
MapPolyline {
    line.color: "#60002200"
    line.width: 1.0
    path: [
        { latitude: 0, longitude: 0  },
        { latitude: 0, longitude: 0  }
    ]

    function hide(){
        line.color = "#00000000"
    }

    function show(){
        line.color = "#60002200"
    }
}