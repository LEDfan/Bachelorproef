#  This is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  any later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#
#  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul

import json
import csv

json_data = open('out.json').read()
data = json.loads(json_data)

provinceToId = {}
idCounter = 0

with open('out.csv' , 'w') as csvfile:
    writer = csv.writer(csvfile, delimiter=",")
    writer.writerow(["id", "province", "population", "x_coord", "y_coord", "latitude", "longitude"])



    for mun in sorted(data, key = lambda key: key['name'] if key['name'] is not None else ""):
        provId = 0

        # skip wrong pages
        if mun['name'] is None:
            continue

        if mun['province'] is None or mun["population"] is None or mun["latitude"] is None or mun["longitude"] is None:
            print("OPGEPAST: ", mun['name'], "is niet compleet!")

        if mun['population']is not None:
            mun['population']= mun['population'].replace(',', '')

        if provinceToId.get(mun["province"], None) is None:
            provId = idCounter
            provinceToId[mun["province"]] = provId
            idCounter += 1
        else:
            provId = provinceToId.get(mun["province"])

        writer.writerow([0, provId, mun["population"], 0, 0, mun["latitude"], mun["longitude"], mun["name"]])
