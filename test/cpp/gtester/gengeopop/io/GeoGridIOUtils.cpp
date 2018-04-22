#include "GeoGridIOUtils.h"
#include <gengeopop/Community.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/School.h>
#include <gengeopop/SecondaryCommunity.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/io/GeoGridProtoReader.h>
#include <gengeopop/io/GeoGridProtoWriter.h>
#include <gtest/gtest.h>

std::map<int, stride::Person*> persons_found;
using namespace gengeopop;

void CompareContactPool(std::shared_ptr<ContactPool>                             contactPool,
                        const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool)
{
        EXPECT_EQ(contactPool->GetID(), protoContactPool.id());
        ASSERT_EQ(protoContactPool.people_size(), (contactPool->end() - contactPool->begin()));
        for (int idx = 0; idx < protoContactPool.people_size(); idx++) {
                auto            personId = protoContactPool.people(idx);
                stride::Person* person   = contactPool->begin()[idx];
                EXPECT_EQ(person->GetId(), personId);
                persons_found[personId] = person;
        }
}

void CompareContactCenter(std::shared_ptr<ContactCenter>               contactCenter,
                          const proto::GeoGrid_Location_ContactCenter& protoContactCenter)
{
        std::map<std::string, proto::GeoGrid_Location_ContactCenter_Type> types = {
            {"School", proto::GeoGrid_Location_ContactCenter_Type_School},
            {"Community", proto::GeoGrid_Location_ContactCenter_Type_Community},
            {"PrimaryCommunity", proto::GeoGrid_Location_ContactCenter_Type_PrimaryCommunity},
            {"SecondaryCommunity", proto::GeoGrid_Location_ContactCenter_Type_SecondaryCommunity},
            {"HighSchool", proto::GeoGrid_Location_ContactCenter_Type_HighSchool},
            {"Household", proto::GeoGrid_Location_ContactCenter_Type_Household},
            {"Workplace", proto::GeoGrid_Location_ContactCenter_Type_Workplace}};

        EXPECT_EQ(contactCenter->GetId(), protoContactCenter.id());
        EXPECT_EQ(types[contactCenter->GetType()], protoContactCenter.type());
        ASSERT_EQ(protoContactCenter.pools_size(), contactCenter->GetPools().size());
        std::map<int, std::shared_ptr<ContactPool>>                      idToPool;
        std::map<int, proto::GeoGrid_Location_ContactCenter_ContactPool> idToProtoPool;
        for (int idx = 0; idx < protoContactCenter.pools_size(); idx++) {
                auto protoContactPool                = protoContactCenter.pools(idx);
                auto contactPool                     = contactCenter->GetPools()[idx];
                idToPool[contactPool->GetID()]       = contactPool;
                idToProtoPool[protoContactPool.id()] = std::move(protoContactPool);
        }
        for (auto& contactPoolPair : idToPool) {
                CompareContactPool(contactPoolPair.second, idToProtoPool[contactPoolPair.first]);
        }
}

void CompareCoordinate(const Coordinate& coordinate, const proto::GeoGrid_Location_Coordinate& protoCoordinate)
{
        EXPECT_EQ(coordinate.x, protoCoordinate.x());
        EXPECT_EQ(coordinate.y, protoCoordinate.y());
        EXPECT_EQ(coordinate.longitude, protoCoordinate.longitude());
        EXPECT_EQ(coordinate.latitude, protoCoordinate.latitude());
}

void CompareLocation(std::shared_ptr<Location> location, const proto::GeoGrid_Location& protoLocation)
{
        EXPECT_EQ(location->GetName(), protoLocation.name());
        EXPECT_EQ(location->GetProvince(), protoLocation.province());
        EXPECT_EQ(location->GetPopulation(), protoLocation.population());
        EXPECT_EQ(location->GetPopulation(), protoLocation.population());
        CompareCoordinate(location->GetCoordinate(), protoLocation.coordinate());
        ASSERT_EQ(protoLocation.contactcenters_size(), location->GetContactCenters().size());

        std::map<int, std::shared_ptr<ContactCenter>>        idToCenter;
        std::map<int, proto::GeoGrid_Location_ContactCenter> idToProtoCenter;

        for (int idx = 0; idx < protoLocation.contactcenters_size(); idx++) {
                auto protoContactCenter                  = protoLocation.contactcenters(idx);
                auto contactCenter                       = location->GetContactCenters()[idx];
                idToCenter[contactCenter->GetId()]       = contactCenter;
                idToProtoCenter[protoContactCenter.id()] = std::move(protoContactCenter);
        }
        for (auto& contactCenterPair : idToCenter) {
                CompareContactCenter(contactCenterPair.second, idToProtoCenter[contactCenterPair.first]);
        }

        ASSERT_EQ(protoLocation.commutes_size(), location->GetOutgoingCommuningCities().size());
        for (int idx = 0; idx < protoLocation.commutes_size(); idx++) {
                auto protoCommute = protoLocation.commutes(idx);
                auto commute_pair = location->GetOutgoingCommuningCities()[idx];
                EXPECT_EQ(protoCommute.to(), commute_pair.first->GetID());
                EXPECT_EQ(protoCommute.proportion(), commute_pair.second);
        }
        ASSERT_EQ(protoLocation.submunicipalities_size(), location->GetSubMunicipalities().size());
        int idx = 0;
        for (auto& submunicipality : location->GetSubMunicipalities()) {
                auto protoSubmunicipalityId = protoLocation.submunicipalities(idx);
                EXPECT_EQ(submunicipality->GetID(), protoSubmunicipalityId);
                idx++;
        }
}
void ComparePerson(const proto::GeoGrid_Person& protoPerson)
{
        auto person = persons_found[protoPerson.id()];
        EXPECT_EQ(person->GetAge(), protoPerson.age());
        EXPECT_EQ(std::string(1, person->GetGender()), protoPerson.gender());
        EXPECT_EQ(person->GetSchoolId(), protoPerson.school());
        EXPECT_EQ(person->GetHouseholdId(), protoPerson.household());
        EXPECT_EQ(person->GetWorkId(), protoPerson.workplace());
        EXPECT_EQ(person->GetPrimaryCommunityId(), protoPerson.primarycommunity());
        EXPECT_EQ(person->GetSecondaryCommunityId(), protoPerson.secondarycommunity());
}

void compareGeoGrid(std::shared_ptr<GeoGrid> geoGrid, proto::GeoGrid& protoGrid)
{
        ASSERT_EQ(geoGrid->size(), protoGrid.locations_size());
        for (int idx = 0; idx < protoGrid.locations_size(); idx++) {
                auto protoLocation = protoGrid.locations(idx);
                auto location      = geoGrid->GetById(protoLocation.id());
                CompareLocation(location, protoLocation);
        }
        ASSERT_EQ(persons_found.size(), protoGrid.persons_size());
        for (int idx = 0; idx < protoGrid.persons_size(); idx++) {
                auto protoPerson = protoGrid.persons(idx);
                ComparePerson(protoPerson);
        }
        persons_found.clear();
}

void CompareGeoGrid(std::shared_ptr<GeoGrid> geoGrid)
{
        GeoGridProtoWriter writer;
        std::stringstream  ss;
        writer.Write(geoGrid, ss);
        proto::GeoGrid protoGrid;
        protoGrid.ParseFromIstream(&ss);
        compareGeoGrid(geoGrid, protoGrid);
}

void CompareGeoGrid(proto::GeoGrid& protoGrid)
{
        std::unique_ptr<std::stringstream> ss = std::make_unique<std::stringstream>();
        protoGrid.SerializeToOstream(ss.get());
        std::unique_ptr<std::istream> is(std::move(ss));

        GeoGridProtoReader       reader(std::move(is));
        std::shared_ptr<GeoGrid> geogrid = reader.Read();
        compareGeoGrid(geogrid, protoGrid);
}

std::shared_ptr<GeoGrid> GetGeoGrid()
{
        GeoGridConfig config{};
        config.input.populationSize        = 10000;
        config.calculated.compulsoryPupils = static_cast<unsigned int>(0.20 * 1000);

        GeoGridGenerator geoGridGenerator(config, std::make_shared<GeoGrid>());
        return geoGridGenerator.GetGeoGrid();
}

std::shared_ptr<GeoGrid> GetPopulatedGeoGrid()
{
        auto geoGrid  = GetGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");

        auto school = std::make_shared<School>(0);
        location->AddContactCenter(school);
        auto schoolPool = std::make_shared<ContactPool>(2, school->GetPoolSize());
        school->AddPool(schoolPool);

        auto community = std::make_shared<PrimaryCommunity>(1);
        location->AddContactCenter(community);
        auto communityPool = std::make_shared<ContactPool>(3, community->GetPoolSize());
        community->AddPool(communityPool);

        auto secondaryCommunity = std::make_shared<SecondaryCommunity>(2);
        location->AddContactCenter(secondaryCommunity);
        auto secondaryCommunityPool = std::make_shared<ContactPool>(7, secondaryCommunity->GetPoolSize());
        secondaryCommunity->AddPool(secondaryCommunityPool);

        auto highschool = std::make_shared<HighSchool>(3);
        location->AddContactCenter(highschool);
        auto highschoolPool = std::make_shared<ContactPool>(4, highschool->GetPoolSize());
        highschool->AddPool(highschoolPool);

        auto household = std::make_shared<Household>(4);
        location->AddContactCenter(household);
        auto householdPool = std::make_shared<ContactPool>(5, household->GetPoolSize());
        household->AddPool(householdPool);

        auto workplace = std::make_shared<Workplace>(5);
        location->AddContactCenter(workplace);
        auto workplacePool = std::make_shared<ContactPool>(6, workplace->GetPoolSize());
        workplace->AddPool(workplacePool);

        geoGrid->AddLocation(location);
        stride::Person* person = geoGrid->CreatePerson(1, 18, 4, 2, 6, 3, 7);
        communityPool->AddMember(person);
        schoolPool->AddMember(person);
        secondaryCommunityPool->AddMember(person);
        highschoolPool->AddMember(person);
        householdPool->AddMember(person);
        workplacePool->AddMember(person);
        return geoGrid;
}

std::shared_ptr<GeoGrid> GetCommutesGeoGrid()
{
        auto geoGrid   = GetGeoGrid();
        auto bavikhove = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        auto gent      = std::make_shared<Location>(2, 4, 2500, Coordinate(0, 0, 0, 0), "Gent");
        auto mons      = std::make_shared<Location>(3, 4, 2500, Coordinate(0, 0, 0, 0), "Mons");

        bavikhove->AddOutgoingCommutingLocation(gent, 0.5);
        gent->AddIncomingCommutingLocation(bavikhove, 0.5);

        bavikhove->AddOutgoingCommutingLocation(mons, 0.25);
        mons->AddIncomingCommutingLocation(bavikhove, 0.25);

        gent->AddOutgoingCommutingLocation(bavikhove, 0.75);
        bavikhove->AddIncomingCommutingLocation(gent, 0.75);

        gent->AddOutgoingCommutingLocation(mons, 0.5);
        mons->AddIncomingCommutingLocation(gent, 0.5);

        geoGrid->AddLocation(bavikhove);
        geoGrid->AddLocation(gent);
        geoGrid->AddLocation(mons);
        return geoGrid;
}
