#include <gengeopop/Community.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/HighSchool.h>
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

std::shared_ptr<GeoGrid> getGeoGrid()
{
        GeoGridConfig config{};
        config.input.populationSize        = 10000;
        config.calculated.compulsoryPupils = static_cast<unsigned int>(0.20 * 1000);

        GeoGridGenerator geoGridGenerator(config, std::make_shared<GeoGrid>());
        return geoGridGenerator.getGeoGrid();
}

TEST(GeoGridProtoWriterTest, locationTest)
{
        auto geoGrid = getGeoGrid();
        geoGrid->addLocation(std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove"));
        geoGrid->addLocation(std::make_shared<Location>(2, 3, 5000, Coordinate(0, 0, 0, 0), "Gent"));
        geoGrid->addLocation(std::make_shared<Location>(3, 2, 2500, Coordinate(0, 0, 0, 0), "Mons"));

        compareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, contactCentersTest)
{
        auto geoGrid  = getGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        location->addContactCenter(std::make_shared<School>(0));
        location->addContactCenter(std::make_shared<Community>(1));
        location->addContactCenter(std::make_shared<HighSchool>(2));
        location->addContactCenter(std::make_shared<Household>(3));
        location->addContactCenter(std::make_shared<Workplace>(4));
        geoGrid->addLocation(location);

        compareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, submunicipalityTest)
{
        auto geoGrid         = getGeoGrid();
        auto location        = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        auto submunicipality = std::make_shared<Location>(2, 4, 2500, Coordinate(0, 0, 0, 0), "Gent");
        submunicipality->addContactCenter(std::make_shared<School>(0));
        location->addSubMunicipality(submunicipality);
        geoGrid->addLocation(location);

        compareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, peopleTest) { compareGeoGrid(getPopulatedGeoGrid()); }
TEST(GeoGridProtoWriterTest, commutesTest) { compareGeoGrid(getCommutesGeoGrid()); }
} // namespace
