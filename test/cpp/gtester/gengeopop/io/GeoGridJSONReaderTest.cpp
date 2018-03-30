#include <fstream>
#include <gtest/gtest.h>
#include <iomanip>
#include <memory>

#include <gengeopop/io/GeoGridJSONReader.h>
#include <util/FileSys.h>

using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> getGeoGridForFile(std::string filename)
{
        std::ifstream file;
        file.open(stride::util::FileSys::GetTestsDir().string() + "/testdata/GeoGridJSON/" + filename);
        GeoGridJSONReader geoGridJSONReader;
        auto              geoGrid = geoGridJSONReader.read(file);
        file.close();
        return geoGrid;
}

TEST(GeoGridJSONReaderTest, locationsTest)
{
        auto geoGrid = getGeoGridForFile("test0.json");

        std::map<unsigned int, std::shared_ptr<Location>> locations;

        locations[geoGrid->get(0)->getID()] = geoGrid->get(0);
        locations[geoGrid->get(1)->getID()] = geoGrid->get(1);
        locations[geoGrid->get(2)->getID()] = geoGrid->get(2);

        auto location1 = locations[1];
        auto location2 = locations[2];
        auto location3 = locations[3];

        EXPECT_EQ(location1->getID(), 1);
        EXPECT_EQ(location1->getName(), "Bavikhove");
        EXPECT_EQ(location1->getProvince(), 4);
        EXPECT_EQ(location1->getPopulation(), 2500);
        EXPECT_EQ(location1->getCoordinate().x, 0);
        EXPECT_EQ(location1->getCoordinate().y, 0);
        EXPECT_EQ(location1->getCoordinate().longitude, 0);
        EXPECT_EQ(location1->getCoordinate().latitude, 0);

        EXPECT_EQ(location2->getID(), 2);
        EXPECT_EQ(location2->getName(), "Gent");
        EXPECT_EQ(location2->getProvince(), 3);
        EXPECT_EQ(location2->getPopulation(), 5000);
        EXPECT_EQ(location2->getCoordinate().x, 0);
        EXPECT_EQ(location2->getCoordinate().y, 0);
        EXPECT_EQ(location2->getCoordinate().longitude, 0);
        EXPECT_EQ(location2->getCoordinate().latitude, 0);

        EXPECT_EQ(location3->getID(), 3);
        EXPECT_EQ(location3->getName(), "Mons");
        EXPECT_EQ(location3->getProvince(), 2);
        EXPECT_EQ(location3->getPopulation(), 2500);
        EXPECT_EQ(location3->getCoordinate().x, 0);
        EXPECT_EQ(location3->getCoordinate().y, 0);
        EXPECT_EQ(location3->getCoordinate().longitude, 0);
        EXPECT_EQ(location3->getCoordinate().latitude, 0);
}

TEST(GeoGridJSONReaderTest, commutesTest)
{
        auto geoGrid = getGeoGridForFile("test7.json");

        std::map<unsigned int, std::shared_ptr<Location>> locations;

        locations[geoGrid->get(0)->getID()] = geoGrid->get(0);
        locations[geoGrid->get(1)->getID()] = geoGrid->get(1);
        locations[geoGrid->get(2)->getID()] = geoGrid->get(2);

        auto location1 = locations[1];
        auto location2 = locations[2];
        auto location3 = locations[3];

        auto sortLoc = [](std::vector<std::pair<std::shared_ptr<Location>, double>> loc) {
                std::sort(std::begin(loc), std::end(loc),
                          [](const std::pair<std::shared_ptr<Location>, double>& a,
                             const std::pair<std::shared_ptr<Location>, double>& b) {
                                  return a.first->getID() < b.first->getID();
                          });
                return loc;
        };

        {
                auto commuting_in  = sortLoc(location1->getIncomingCommuningCities());
                auto commuting_out = sortLoc(location1->getOutgoingCommuningCities());
                EXPECT_EQ(commuting_in.size(), 1);
                EXPECT_EQ(commuting_out.size(), 2);

                EXPECT_EQ(commuting_in[0].first->getID(), 2);
                EXPECT_EQ(commuting_in[0].second, 42);

                EXPECT_EQ(commuting_out[0].first->getID(), 2);
                EXPECT_EQ(commuting_out[0].second, 20);
                EXPECT_EQ(commuting_out[1].first->getID(), 3);
                EXPECT_EQ(commuting_out[1].second, 1.5);
        }
        {
                auto commuting_in  = sortLoc(location2->getIncomingCommuningCities());
                auto commuting_out = sortLoc(location2->getOutgoingCommuningCities());
                EXPECT_EQ(commuting_out.size(), 2);
                EXPECT_EQ(commuting_in.size(), 1);

                EXPECT_EQ(commuting_in[0].first->getID(), 1);
                EXPECT_EQ(commuting_in[0].second, 20);

                EXPECT_EQ(commuting_out[0].first->getID(), 1);
                EXPECT_EQ(commuting_out[0].second, 42);
                EXPECT_EQ(commuting_out[1].first->getID(), 3);
                EXPECT_EQ(commuting_out[1].second, 2);
        }
        {
                auto commuting_in  = sortLoc(location3->getIncomingCommuningCities());
                auto commuting_out = sortLoc(location3->getOutgoingCommuningCities());
                EXPECT_EQ(commuting_out.size(), 0);
                EXPECT_EQ(commuting_in.size(), 2);

                EXPECT_EQ(commuting_in[0].first->getID(), 1);
                EXPECT_EQ(commuting_in[0].second, 1.5);
                EXPECT_EQ(commuting_in[1].first->getID(), 2);
                EXPECT_EQ(commuting_in[1].second, 2);
        }
}

TEST(GeoGridJSONReaderTest, contactCentersTest)
{
        auto                        geoGrid        = getGeoGridForFile("test1.json");
        auto                        location       = geoGrid->get(0);
        auto                        contactCenters = location->getContactCenters();
        std::map<std::string, bool> found          = {
            {"School", false}, {"Community", false}, {"HighSchool", false}, {"Household", false}, {"Workplace", false}};

        for (unsigned int i = 0; i < 5; i++) {
                EXPECT_FALSE(found[contactCenters[i]->getType()]);
                found[contactCenters[i]->getType()] = true;
        }
        for (auto& type : found) {
                EXPECT_TRUE(type.second);
        }
}

void runPeopleTest(std::string filename)
{
        auto                       geoGrid  = getGeoGridForFile(filename);
        auto                       location = geoGrid->get(0);
        std::map<int, std::string> types    = {{2, "School"},     {3, "Community"}, {7, "Community"},
                                            {4, "HighSchool"}, {5, "Household"}, {6, "Workplace"}};

        EXPECT_EQ(location->getID(), 1);
        EXPECT_EQ(location->getName(), "Bavikhove");
        EXPECT_EQ(location->getProvince(), 4);
        EXPECT_EQ(location->getPopulation(), 2500);
        EXPECT_EQ(location->getCoordinate().x, 0);
        EXPECT_EQ(location->getCoordinate().y, 0);
        EXPECT_EQ(location->getCoordinate().longitude, 0);
        EXPECT_EQ(location->getCoordinate().latitude, 0);

        auto contactCenters = location->getContactCenters();
        for (const auto& center : contactCenters) {
                auto pool   = center->GetPools()[0];
                auto person = *(pool->begin());
                EXPECT_EQ(types[pool->getID()], center->getType());
                EXPECT_EQ(person->GetId(), 1);
                EXPECT_EQ(person->GetAge(), 18);
                EXPECT_EQ(person->GetGender(), 'M');
                EXPECT_EQ(person->GetSchoolId(), 2);
                EXPECT_EQ(person->GetHouseholdId(), 4);
                EXPECT_EQ(person->GetWorkId(), 6);
                EXPECT_EQ(person->GetPrimaryCommunityId(), 3);
                EXPECT_EQ(person->GetSecondaryCommunityId(), 7);
        }
}

TEST(GeoGridJSONReaderTest, peopleTest) { runPeopleTest("test2.json"); }

TEST(GeoGridJSONReaderTest, intTest) { runPeopleTest("test3.json"); }

TEST(GeoGridJSONReaderTest, emptyStreamTest)
{
        GeoGridJSONReader geoGridJSONReader;
        std::stringstream ss;
        EXPECT_THROW(geoGridJSONReader.read(ss), std::runtime_error);
}

TEST(GeoGridJSONReaderTest, invalidTypeTest) { EXPECT_THROW(getGeoGridForFile("test4.json"), std::invalid_argument); }

TEST(GeoGridJSONReaderTest, invalidPersonTest) { EXPECT_THROW(getGeoGridForFile("test5.json"), std::invalid_argument); }

TEST(GeoGridJSONReaderTest, invalidJSONTest) { EXPECT_THROW(getGeoGridForFile("test6.json"), std::runtime_error); }

} // namespace
