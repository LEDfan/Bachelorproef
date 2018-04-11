#include "compareProtoGeoGrid.h"
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/io/GeoGridProtoReader.h>
#include <gengeopop/io/GeoGridProtoWriter.h>
#include <gtest/gtest.h>

std::map<int, std::shared_ptr<stride::Person>> persons_found;
using namespace gengeopop;

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
        std::map<int, std::shared_ptr<ContactPool>>                      idToPool;
        std::map<int, proto::GeoGrid_Location_ContactCenter_ContactPool> idToProtoPool;
        for (int idx = 0; idx < protoContactCenter.pools_size(); idx++) {
                auto protoContactPool                = protoContactCenter.pools(idx);
                auto contactPool                     = contactCenter->GetPools()[idx];
                idToPool[contactPool->getID()]       = contactPool;
                idToProtoPool[protoContactPool.id()] = std::move(protoContactPool);
        }
        for (auto& contactPoolPair : idToPool) {
                compareContactPool(contactPoolPair.second, idToProtoPool[contactPoolPair.first]);
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

        std::map<int, std::shared_ptr<ContactCenter>>        idToCenter;
        std::map<int, proto::GeoGrid_Location_ContactCenter> idToProtoCenter;

        for (int idx = 0; idx < protoLocation.contactcenters_size(); idx++) {
                auto protoContactCenter                  = protoLocation.contactcenters(idx);
                auto contactCenter                       = location->getContactCenters()[idx];
                idToCenter[contactCenter->getId()]       = contactCenter;
                idToProtoCenter[protoContactCenter.id()] = std::move(protoContactCenter);
        }
        for (auto& contactCenterPair : idToCenter) {
                compareContactCenter(contactCenterPair.second, idToProtoCenter[contactCenterPair.first]);
        }

        ASSERT_EQ(protoLocation.commutes_size(), location->getOutgoingCommuningCities().size());
        for (int idx = 0; idx < protoLocation.commutes_size(); idx++) {
                auto protoCommute = protoLocation.commutes(idx);
                auto commute_pair = location->getOutgoingCommuningCities()[idx];
                EXPECT_EQ(protoCommute.to(), commute_pair.first->getID());
                EXPECT_EQ(protoCommute.proportion(), commute_pair.second);
        }
        ASSERT_EQ(protoLocation.submunicipalities_size(), location->getSubMunicipalities().size());
        int idx = 0;
        for (auto& submunicipality : location->getSubMunicipalities()) {
                auto protoSubmunicipality = protoLocation.submunicipalities(idx);
                compareLocation(submunicipality, protoSubmunicipality);
                idx++;
        }
}
void comparePerson(const proto::GeoGrid_Person& protoPerson)
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
                compareLocation(location, protoLocation);
        }
        ASSERT_EQ(persons_found.size(), protoGrid.persons_size());
        for (int idx = 0; idx < protoGrid.persons_size(); idx++) {
                auto protoPerson = protoGrid.persons(idx);
                comparePerson(protoPerson);
        }
        persons_found.clear();
}

void compareGeoGrid(std::shared_ptr<GeoGrid> geoGrid)
{
        GeoGridProtoWriter writer;
        std::stringstream  ss;
        writer.write(geoGrid, ss);
        proto::GeoGrid protoGrid;
        protoGrid.ParseFromIstream(&ss);
        compareGeoGrid(geoGrid, protoGrid);
}

void compareGeoGrid(proto::GeoGrid& protoGrid)
{
        GeoGridProtoReader reader;
        std::stringstream  ss;
        protoGrid.SerializeToOstream(&ss);
        std::shared_ptr<GeoGrid> geogrid = reader.read(ss);
        compareGeoGrid(geogrid, protoGrid);
}
