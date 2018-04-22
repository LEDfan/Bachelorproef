#pragma once

#include <gengeopop/ContactCenter.h>
#include <gengeopop/ContactPool.h>
#include <gengeopop/Coordinate.h>
#include <gengeopop/GeoGrid.h>
#include <gengeopop/io/proto/geogrid.pb.h>

using namespace gengeopop;

void CompareContactPool(std::shared_ptr<ContactPool>                             contactPool,
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
