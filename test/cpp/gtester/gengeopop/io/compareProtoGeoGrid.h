#pragma once

#include <gengeopop/ContactCenter.h>
#include <gengeopop/ContactPool.h>
#include <gengeopop/Coordinate.h>
#include <gengeopop/GeoGrid.h>
#include <gengeopop/io/proto/geogrid.pb.h>

using namespace gengeopop;

void compareContactPool(std::shared_ptr<ContactPool>                             contactPool,
                        const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool);

void compareContactCenter(std::shared_ptr<ContactCenter>               contactCenter,
                          const proto::GeoGrid_Location_ContactCenter& protoContactCenter);

void compareCoordinate(const Coordinate& coordinate, const proto::GeoGrid_Location_Coordinate& protoCoordinate);

void compareLocation(std::shared_ptr<Location> location, const proto::GeoGrid_Location& protoLocation);

void comparePerson(const proto::GeoGrid_Person& protoPerson);

void CompareProtoGeoGrid(proto::GeoGrid& protoGeoGrid);

void compareGeoGrid(std::shared_ptr<GeoGrid> geoGrid);

void compareGeoGrid(proto::GeoGrid& protoGrid);
