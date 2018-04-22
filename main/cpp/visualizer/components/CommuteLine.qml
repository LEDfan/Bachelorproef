import QtQuick 2.0;
import QtLocation 5.3;

MapPolyline {
    line.color: "#60002200"
    line.width: 1.0
    id: mapLine
    path: [
        { latitude: 0, longitude: 0  },
        { latitude: 0, longitude: 0  }
    ]

    function Hide(){
        line.color = "#00000000"
    }

    function Show(){
        line.color = "#60002200"
    }

}