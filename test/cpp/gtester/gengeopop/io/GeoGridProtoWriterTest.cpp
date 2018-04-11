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

#include "compareProtoGeoGrid.h"
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
TEST(GeoGridProtoWriterTest, peopleTest)
{
        auto geoGrid  = getGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");

        auto school = std::make_shared<School>(0);
        location->addContactCenter(school);
        auto schoolPool = std::make_shared<ContactPool>(2, school->getPoolSize());
        school->addPool(schoolPool);

        auto community = std::make_shared<PrimaryCommunity>(1);
        location->addContactCenter(community);
        auto communityPool = std::make_shared<ContactPool>(3, community->getPoolSize());
        community->addPool(communityPool);

        auto secondaryCommunity = std::make_shared<SecondaryCommunity>(2);
        location->addContactCenter(secondaryCommunity);
        auto secondaryCommunityPool = std::make_shared<ContactPool>(7, secondaryCommunity->getPoolSize());
        secondaryCommunity->addPool(secondaryCommunityPool);

        auto highschool = std::make_shared<HighSchool>(3);
        location->addContactCenter(highschool);
        auto highschoolPool = std::make_shared<ContactPool>(4, highschool->getPoolSize());
        highschool->addPool(highschoolPool);

        auto household = std::make_shared<Household>(4);
        location->addContactCenter(household);
        auto householdPool = std::make_shared<ContactPool>(5, household->getPoolSize());
        household->addPool(householdPool);

        auto workplace = std::make_shared<Workplace>(5);
        location->addContactCenter(workplace);
        auto workplacePool = std::make_shared<ContactPool>(6, workplace->getPoolSize());
        workplace->addPool(workplacePool);

        geoGrid->addLocation(location);
        auto person = std::make_shared<stride::Person>(1, 18, 4, 2, 6, 3, 7, 0, 0, 0, 0, 0);
        communityPool->addMember(person);
        schoolPool->addMember(person);
        secondaryCommunityPool->addMember(person);
        highschoolPool->addMember(person);
        householdPool->addMember(person);
        workplacePool->addMember(person);

        compareGeoGrid(geoGrid);
}
TEST(GeoGridProtoWriterTest, commutesTest)
{
        auto geoGrid   = getGeoGrid();
        auto bavikhove = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        auto gent      = std::make_shared<Location>(2, 4, 2500, Coordinate(0, 0, 0, 0), "Gent");
        auto mons      = std::make_shared<Location>(3, 4, 2500, Coordinate(0, 0, 0, 0), "Mons");

        bavikhove->addOutgoingCommutingLocation(gent, 0.5);
        gent->addIncomingCommutingLocation(bavikhove, 0.5);

        bavikhove->addOutgoingCommutingLocation(mons, 0.25);
        mons->addIncomingCommutingLocation(bavikhove, 0.25);

        gent->addOutgoingCommutingLocation(bavikhove, 0.75);
        bavikhove->addIncomingCommutingLocation(gent, 0.75);

        gent->addOutgoingCommutingLocation(mons, 0.5);
        mons->addIncomingCommutingLocation(gent, 0.5);

        geoGrid->addLocation(bavikhove);
        geoGrid->addLocation(gent);
        geoGrid->addLocation(mons);

        compareGeoGrid(geoGrid);
}
} // namespace
