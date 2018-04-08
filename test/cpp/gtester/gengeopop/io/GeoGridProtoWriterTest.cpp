#include <gengeopop/Community.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/io/GeoGridProtoWriter.h>
#include <gengeopop/io/proto/geogrid.pb.h>
#include <gtest/gtest.h>
#include <util/FileSys.h>

using namespace gengeopop;

namespace {

std::map<int, std::shared_ptr<stride::Person>> persons_found;

std::shared_ptr<GeoGrid> getGeoGrid()
{
        GeoGridConfig config{};
        config.input.populationSize        = 10000;
        config.calculated.compulsoryPupils = static_cast<unsigned int>(0.20 * 1000);

        GeoGridGenerator geoGridGenerator(config, std::make_shared<GeoGrid>());
        return geoGridGenerator.getGeoGrid();
}
void compareContactPool(std::shared_ptr<ContactPool>                             contactPool,
                        const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool)
{
        EXPECT_EQ(contactPool->getID(), protoContactPool.id());
        ASSERT_EQ(protoContactPool.people_size(), (contactPool->end() - contactPool->begin()));
        for (int idx = 0; idx < protoContactPool.people_size(); idx++) {
                auto                            personId = protoContactPool.people(idx);
                std::shared_ptr<stride::Person> person   = contactPool->begin()[idx];
                EXPECT_EQ(person->GetId(), personId);
                persons_found[personId] = person;
        }
}

void compareContactCenter(std::shared_ptr<ContactCenter>               contactCenter,
                          const proto::GeoGrid_Location_ContactCenter& protoContactCenter)
{
        EXPECT_EQ(contactCenter->getId(), protoContactCenter.id());
        EXPECT_EQ(contactCenter->getType(), protoContactCenter.type());
        ASSERT_EQ(protoContactCenter.pools_size(), contactCenter->GetPools().size());
        for (int idx = 0; idx < protoContactCenter.pools_size(); idx++) {
                auto protoContactPool = protoContactCenter.pools(idx);
                auto contactPool      = contactCenter->GetPools()[idx];
                compareContactPool(contactPool, protoContactPool);
        }
}

void compareCoordinate(const Coordinate& coordinate, const proto::GeoGrid_Location_Coordinate& protoCoordinate)
{
        EXPECT_EQ(coordinate.x, protoCoordinate.x());
        EXPECT_EQ(coordinate.y, protoCoordinate.y());
        EXPECT_EQ(coordinate.longitude, protoCoordinate.longitude());
        EXPECT_EQ(coordinate.latitude, protoCoordinate.latitude());
}

void compareLocation(std::shared_ptr<Location> location, const proto::GeoGrid_Location& protoLocation)
{
        EXPECT_EQ(location->getName(), protoLocation.name());
        EXPECT_EQ(location->getProvince(), protoLocation.province());
        EXPECT_EQ(location->getPopulation(), protoLocation.population());
        EXPECT_EQ(location->getPopulation(), protoLocation.population());
        compareCoordinate(location->getCoordinate(), protoLocation.coordinate());
        ASSERT_EQ(protoLocation.contactcenters_size(), location->getContactCenters().size());
        for (int idx = 0; idx < protoLocation.contactcenters_size(); idx++) {
                auto protoContactCenter = protoLocation.contactcenters(idx);
                auto contactCenter      = location->getContactCenters()[idx];
                compareContactCenter(contactCenter, protoContactCenter);
        }
        ASSERT_EQ(protoLocation.commutes_size(), location->getOutgoingCommuningCities().size());
        for (int idx = 0; idx < protoLocation.commutes_size(); idx++) {
                auto protoCommute = protoLocation.commutes(idx);
                auto commute_pair = location->getOutgoingCommuningCities()[idx];
                EXPECT_EQ(protoCommute.to(), commute_pair.first->getID());
                EXPECT_EQ(protoCommute.proportion(), commute_pair.second);
        }
}
void comparePerson(const proto::GeoGrid_Person& protoPerson)
{
        auto person = persons_found[protoPerson.id()];
        EXPECT_EQ(person->GetAge(), protoPerson.age());
        EXPECT_EQ(std::to_string(person->GetGender()), protoPerson.gender());
        EXPECT_EQ(person->GetSchoolId(), protoPerson.school());
        EXPECT_EQ(person->GetHouseholdId(), protoPerson.household());
        EXPECT_EQ(person->GetWorkId(), protoPerson.workplace());
        EXPECT_EQ(person->GetPrimaryCommunityId(), protoPerson.primarycommunity());
        EXPECT_EQ(person->GetSecondaryCommunityId(), protoPerson.secondarycommunity());
}

void compareGeoGrid(std::shared_ptr<GeoGrid> geoGrid)
{
        GeoGridProtoWriter writer;
        std::stringstream  ss;
        writer.write(geoGrid, ss);
        proto::GeoGrid protoGrid;
        protoGrid.ParseFromIstream(&ss);
        ASSERT_EQ(geoGrid->size(), protoGrid.locations_size());
        for (int idx = 0; idx < protoGrid.locations_size(); idx++) {
                auto protoLocation = protoGrid.locations(idx);
                auto location      = geoGrid->GetById(protoLocation.id());
                compareLocation(location, protoLocation);
        }
        ASSERT_EQ(persons_found.size(), protoGrid.persons_size());
        for (int idx = 0; idx < protoGrid.persons_size(); idx++) {
                auto protoPerson = protoGrid.persons(idx);
                comparePerson(protoPerson);
        }
        persons_found.clear();
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
TEST(GeoGridProtoWriterTest, peopleTest)
{
        auto geoGrid  = getGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");

        auto school = std::make_shared<School>(0);
        location->addContactCenter(school);
        auto schoolPool = std::make_shared<ContactPool>(2, school->getPoolSize());
        school->addPool(schoolPool);

        auto community = std::make_shared<Community>(1);
        location->addContactCenter(community);
        auto communityPool = std::make_shared<ContactPool>(3, community->getPoolSize());
        community->addPool(communityPool);

        auto secondaryCommunity = std::make_shared<Community>(2);
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
