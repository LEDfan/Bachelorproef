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

#pragma once

#include <gengeopop/ContactCenter.h>
#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <gengeopop/io/proto/geogrid.pb.h>

using namespace gengeopop;

void CompareContactPool(stride::ContactPool*                                     contactPool,
                        const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool);

void CompareContactCenter(std::shared_ptr<ContactCenter>               contactCenter,
                          const proto::GeoGrid_Location_ContactCenter& protoContactCenter);

void CompareCoordinate(const Coordinate& coordinate, const proto::GeoGrid_Location_Coordinate& protoCoordinate);

void CompareLocation(std::shared_ptr<Location> location, const proto::GeoGrid_Location& protoLocation);

void ComparePerson(const proto::GeoGrid_Person& protoPerson);

void CompareProtoGeoGrid(proto::GeoGrid& protoGeoGrid);

void CompareGeoGrid(std::shared_ptr<GeoGrid> geoGrid);

void CompareGeoGrid(proto::GeoGrid& protoGrid);

std::shared_ptr<GeoGrid> GetPopulatedGeoGrid();

std::shared_ptr<GeoGrid> GetGeoGrid();

std::shared_ptr<GeoGrid> GetCommutesGeoGrid();
