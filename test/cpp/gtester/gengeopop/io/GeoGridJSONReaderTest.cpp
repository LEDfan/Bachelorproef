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

        auto location1 = geoGrid->get(0);
        auto location2 = geoGrid->get(1);
        auto location3 = geoGrid->get(2);

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

TEST(GeoGridJSONReaderTest, contactCentersTest)
{
        auto geoGrid        = getGeoGridForFile("test1.json");
        auto location       = geoGrid->get(0);
        auto contactCenters = location->getContactCenters();
        EXPECT_EQ(contactCenters[0]->getType(), "School");
        EXPECT_EQ(contactCenters[1]->getType(), "Community");
        EXPECT_EQ(contactCenters[2]->getType(), "HighSchool");
        EXPECT_EQ(contactCenters[3]->getType(), "Household");
        EXPECT_EQ(contactCenters[4]->getType(), "Workplace");
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
