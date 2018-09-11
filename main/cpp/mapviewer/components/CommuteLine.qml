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

    function hide(){
        line.color = "#00000000"
    }

    function show(){
        line.color = "#60002200"
    }

}
