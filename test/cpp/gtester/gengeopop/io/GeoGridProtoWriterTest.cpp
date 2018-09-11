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

#include <gengeopop/College.h>
#include <gengeopop/Community.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/K12School.h>
#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/SecondaryCommunity.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/io/GeoGridProtoWriter.h>
#include <gengeopop/io/proto/geogrid.pb.h>
#include <gtest/gtest.h>
#include <util/FileSys.h>

#include "GeoGridIOUtils.h"
using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> GetGeoGrid()
{
        GeoGridConfig config{};
        config.input.populationSize        = 10000;
        config.calculated.compulsoryPupils = static_cast<unsigned int>(0.20 * 1000);

        GeoGridGenerator geoGridGenerator(config, std::make_shared<GeoGrid>());
        return geoGridGenerator.GetGeoGrid();
}

TEST(GeoGridProtoWriterTest, locationTest)
{
        auto geoGrid = GetGeoGrid();
        geoGrid->AddLocation(std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0), "Bavikhove"));
        geoGrid->AddLocation(std::make_shared<Location>(2, 3, 5000, Coordinate(0, 0), "Gent"));
        geoGrid->AddLocation(std::make_shared<Location>(3, 2, 2500, Coordinate(0, 0), "Mons"));

        CompareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, contactCentersTest)
{
        auto geoGrid  = GetGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0), "Bavikhove");
        location->AddContactCenter(std::make_shared<K12School>(0));
        location->AddContactCenter(std::make_shared<PrimaryCommunity>(1));
        location->AddContactCenter(std::make_shared<College>(2));
        location->AddContactCenter(std::make_shared<Household>(3));
        location->AddContactCenter(std::make_shared<Workplace>(4));
        geoGrid->AddLocation(location);

        CompareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, peopleTest) { CompareGeoGrid(GetPopulatedGeoGrid()); }
TEST(GeoGridProtoWriterTest, commutesTest) { CompareGeoGrid(GetCommutesGeoGrid()); }
} // namespace
