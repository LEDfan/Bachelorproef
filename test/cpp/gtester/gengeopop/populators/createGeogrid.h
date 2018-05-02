#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/GeoGridConfig.h>

std::shared_ptr<gengeopop::GeoGrid> CreateGeoGrid(int locCount, int locPop, int schoolCount, int houseHoldCount,
                                                  int personCount, gengeopop::GeoGridConfig& geoGridConfig);
