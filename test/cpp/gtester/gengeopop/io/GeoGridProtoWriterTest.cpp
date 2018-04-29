#include <gengeopop/College.h>
#include <gengeopop/Community.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/School.h>
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
        geoGrid->AddLocation(std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove"));
        geoGrid->AddLocation(std::make_shared<Location>(2, 3, 5000, Coordinate(0, 0, 0, 0), "Gent"));
        geoGrid->AddLocation(std::make_shared<Location>(3, 2, 2500, Coordinate(0, 0, 0, 0), "Mons"));

        CompareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, contactCentersTest)
{
        auto geoGrid  = GetGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        location->AddContactCenter(std::make_shared<School>(0));
        location->AddContactCenter(std::make_shared<Community>(1));
        location->AddContactCenter(std::make_shared<College>(2));
        location->AddContactCenter(std::make_shared<Household>(3));
        location->AddContactCenter(std::make_shared<Workplace>(4));
        geoGrid->AddLocation(location);

        CompareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, submunicipalityTest)
{
        auto geoGrid         = GetGeoGrid();
        auto location        = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        auto submunicipality = std::make_shared<Location>(2, 4, 2500, Coordinate(0, 0, 0, 0), "Gent");
        submunicipality->AddContactCenter(std::make_shared<School>(0));
        location->AddSubMunicipality(submunicipality);
        geoGrid->AddLocation(location);

        CompareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, peopleTest) { CompareGeoGrid(GetPopulatedGeoGrid()); }
TEST(GeoGridProtoWriterTest, commutesTest) { CompareGeoGrid(GetCommutesGeoGrid()); }
} // namespace
