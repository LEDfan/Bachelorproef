# coding=utf-8
import csv
import json
import re
import math
from pprint import pprint

input_data = json.load(open('output.json'))

data = {}

def parseCoordinate(coord):
    lat_degree, lat_minute = coord.strip().split(' ')
    lat_degree = lat_degree.strip()
    lat_minute = lat_minute.strip()
    return str(int(lat_degree) + int(lat_minute) / 60.0)


# parse the scarped data
for submunicipality in input_data:

    if 'nis' in submunicipality:
        continue

    if 'main' in submunicipality and submunicipality['main']:
        try:
            data[submunicipality['name']]['has_main'] = True
        except:
            data[submunicipality['name']] = {'has_main': True, 'children': [], 'population': None}
        continue

    if submunicipality['child']['name'] == submunicipality['parent_name']:
        try:
            data[submunicipality['child']['name']]['has_main'] = True
        except:
            data[submunicipality['child']['name']] = {'has_main': True, 'children': [], 'population': None}
        continue

    child = submunicipality['child']
    if child['name'] == 'Antwerpen': continue

    if child['area']:
        child['area'] = child['area'].replace('km²', '').strip()
    if child['coordinates']:
        lon, lat = child['coordinates'].replace('°', '').replace("′", '').replace("NB", '').replace("OL", '').strip().split(',')
        child['lat'] = parseCoordinate(lat)
        child['lon'] = parseCoordinate(lon)
        del child['coordinates']

    if child['name']:
        child['name'] = re.sub('\(.*\)', '', child['name']).strip()

    if child['population']:
        child['population'] = re.sub('\(.*\)*', '', child['population'])
        child['population'] = re.sub('[a-zA-Z]*', '', child['population'])
        child['population'] = int(child['population'].replace('.', '').replace(' ', ''))

    if submunicipality['parent_population']:
        submunicipality['parent_population'] = re.sub('\(.*\)*', '', submunicipality['parent_population'])
        submunicipality['parent_population'] = re.sub('[a-zA-Z]*', '', submunicipality['parent_population'])
        submunicipality['parent_population'] = int(submunicipality['parent_population'].replace('.', '').replace(' ', ''))

    try:
        data[submunicipality['parent_name']]['children'].append(submunicipality['child'])
        data[submunicipality['parent_name']]['population'] = submunicipality['parent_population']
    except:
        data[submunicipality['parent_name']] = {'children': [submunicipality['child']], 'population': submunicipality['parent_population'], 'has_main': False}


# parse nis,lon and lat of parent data
for el in input_data:
    if 'nis' in el:
        name = re.sub('\(.*\)', '', el['name']).strip()
        data[name]['nis'] = el['nis'].strip()
        lon, lat = el['coordinates'].replace('°', '').replace("′", '').replace("NB", '').replace("OL", '').replace("'", '').strip().split(',')
        data[name]['lon'] = parseCoordinate(lon)
        data[name]['lat'] = parseCoordinate(lat)

# this part will scale the population of the sub-municipalities because the data of the population is relative old
for name, parent in data.items():
    populationChildren = 0
    countChildren = len([x for x in parent['children'] if x['name'] != name])
    populationParent = parent['population']

    if countChildren == 0: continue

    if parent['has_main']:
        # the parent city is a sub-municipality on it's own (e.g. Antwerpen)
        parenCitySize = 1
    else:
        # the parent city is NOT a sub-municipality on it's own (e.g. Malle (Oost-Malle and West-Malle)
        parenCitySize = 0

    # divide the actual population of the parent in parts
    # take two parts for the main city and one part for every sub-municipality
    P = populationParent / (countChildren + parenCitySize)

    # sometimes the population of a sub-municipality is unknown hence calculate the average of every known sub-municipality of this city
    notNonePopulationChildren = [x['population'] for x in parent['children'] if x['population'] is not None]
    if len(notNonePopulationChildren) != 0:
        averagePopulation = math.floor(sum(notNonePopulationChildren) / len(notNonePopulationChildren))
    else:
        # if the population of every sub-municipality is unknown take P as average
        averagePopulation = P

    # if the floor cause 0 -> take average
    if averagePopulation == 0:
        averagePopulation = P

    # calculate the total population of the sub-municipalities + assign average of the population is unknown
    for child in parent['children']:
        if child['name'] != name and child['population'] is not None:
            populationChildren += child['population']
        elif child['name'] != name and child['population'] is None:
            child['population'] = averagePopulation
            populationChildren += child['population']

    # the part of the population of the parent city occupied by the children
    shareSubMuns = populationParent - parenCitySize * P

    totalRel = 0
    for child in parent['children']:
        # for every sub-municipality
        # calculate the relative size of this sub-municipality to the total population of all sub-municipalities
        # make the relative size absolute w.r.t the actual parent city = SCALE UP
        # make it again relative to the total population of the parent city
        child['scaled_population'] = (child['population'] / populationChildren) * shareSubMuns
        child['rel_to_parent'] = child['scaled_population'] / populationParent
        totalRel += child['rel_to_parent']

    if parent['has_main']:
        parent['children'].append({
            'rel_to_parent': 1 - totalRel,
            'lat':  parent['lat'],
            'lon':  parent['lon'],
            'name': name
        })


# pprint(data)

# write data to file

with open('submunicipalities.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['parent_nis', 'nis', 'parent_is_sub', 'population_rel_to_parent', 'latitude', 'longitude', 'name'])
    for parent in data.values():
        # if parent['has_main']:
            # the parent is a city sub-municipality itself
            # writer.writerow([parent['nis'], str(parent['nis']) + "000" + str(i), parent['has_main'], child['rel_to_parent'], child['lat'], child['lon'], child['name'].upper()])

        i = 1
        for child in parent['children']:
            writer.writerow([parent['nis'], str(parent['nis']) + "000" + str(i), parent['has_main'], child['rel_to_parent'], child['lat'], child['lon'], child['name'].upper()])
            i += 1



# testcity = 'Schilde'
#
# print("Total population", data[testcity]['population'])
# for city in data[testcity]['children']:
#     if city['name'] == testcity:
#         continue
#     print(city['name'], '\t', city['population'], '\t', city['scaled_population'])
#
#
# print("Calculated sub population:",sum(city['scaled_population'] for city in data[testcity]['children'] if city['name'] != testcity))
# print("Calculated main city population:", data[testcity]['population'] - sum(city['scaled_population'] for city in data[testcity]['children'] if city['name'] != testcity))
# print("Sub-municipalities count:", len([city['scaled_population'] for city in data[testcity]['children'] if city['name'] != testcity]))


