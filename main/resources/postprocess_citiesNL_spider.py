import json
import csv

json_data = open('out.json').read()
data = json.loads(json_data)

provinceToId = {}
idCounter = 0

with open('out.csv' , 'w') as csvfile:
    writer = csv.writer(csvfile, delimiter=",")
    writer.writerow(["id", "province", "population", "x_coord", "y_coord", "latitude", "longitude"])

    for mun in data:
        provId = 0
        if provinceToId.get(mun["province"], None) is None:
            provId = idCounter
            provinceToId[mun["province"]] = provId
            idCounter += 1
        else:
            provId = provinceToId.get(mun["province"])

        writer.writerow([0, provId, mun["population"], 0, 0, mun["latitude"], mun["longitude"], mun["name"]])
