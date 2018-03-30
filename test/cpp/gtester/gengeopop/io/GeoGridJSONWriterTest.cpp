#include <gengeopop/Community.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <gtest/gtest.h>
#include <util/FileSys.h>

using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> getGeoGrid()
{
        GeoGridConfig config;
        config.input_populationSize  = 10000;
        config.calc_compulsoryPupils = static_cast<unsigned int>(0.20 * 1000);

        GeoGridGenerator geoGridGenerator(config, std::make_shared<GeoGrid>());
        return geoGridGenerator.getGeoGrid();
}

void sortContactCenters(boost::property_tree::ptree& tree)
{
        auto& contactCenters       = tree.get_child("contactCenters");
        auto  compareContactCenter = [](const std::pair<std::string, boost::property_tree::ptree>& a,
                                       const std::pair<std::string, boost::property_tree::ptree>& b) {
                return a.second.get<std::string>("type") < b.second.get<std::string>("type");
        };
        contactCenters.sort<decltype(compareContactCenter)>(compareContactCenter);
}

void sortTree(boost::property_tree::ptree& tree)
{
        auto compareLocation = [](const std::pair<std::string, boost::property_tree::ptree>& a,
                                  const std::pair<std::string, boost::property_tree::ptree>& b) {
                return a.second.get<std::string>("id") < b.second.get<std::string>("id");
        };
        auto& locations = tree.get_child("locations");
        locations.sort<decltype(compareLocation)>(compareLocation);

        for (auto it = locations.begin(); it != locations.end(); it++) {
                sortContactCenters(it->second.get_child(""));
        }
}

bool compareGeoGrid(std::shared_ptr<GeoGrid> geoGrid, std::string testname)
{
        GeoGridJSONWriter writer;
        std::stringstream ss;
        writer.write(geoGrid, ss);

        boost::property_tree::ptree result;
        boost::property_tree::read_json(ss, result);
        sortTree(result);

        boost::property_tree::ptree expected;
        boost::property_tree::read_json(
            stride::util::FileSys::GetTestsDir().string() + "/testdata/GeoGridJSON/" + testname, expected);
        sortTree(expected);

        return result == expected;
}

TEST(GeoGridJSONWriterTest, locationTest)
{
        auto geoGrid = getGeoGrid();
        geoGrid->addLocation(std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove"));
        geoGrid->addLocation(std::make_shared<Location>(2, 3, 5000, Coordinate(0, 0, 0, 0), "Gent"));
        geoGrid->addLocation(std::make_shared<Location>(3, 2, 2500, Coordinate(0, 0, 0, 0), "Mons"));

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test0.json"));
}
TEST(GeoGridJSONWriterTest, contactCentersTest)
{
        auto geoGrid  = getGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        location->addContactCenter(std::make_shared<School>());
        location->addContactCenter(std::make_shared<Community>());
        location->addContactCenter(std::make_shared<HighSchool>());
        location->addContactCenter(std::make_shared<Household>());
        location->addContactCenter(std::make_shared<Workplace>());
        geoGrid->addLocation(location);

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test1.json"));
}
TEST(GeoGridJSONWriterTest, peopleTest)
{
        auto geoGrid  = getGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");

        auto school = std::make_shared<School>();
        location->addContactCenter(school);
        auto schoolPool = std::make_shared<ContactPool>(2);
        school->addPool(schoolPool);

        auto community = std::make_shared<Community>();
        location->addContactCenter(community);
        auto communityPool = std::make_shared<ContactPool>(3);
        community->addPool(communityPool);

        auto secondaryCommunity = std::make_shared<Community>();
        location->addContactCenter(secondaryCommunity);
        auto secondaryCommunityPool = std::make_shared<ContactPool>(7);
        secondaryCommunity->addPool(secondaryCommunityPool);

        auto highschool = std::make_shared<HighSchool>();
        location->addContactCenter(highschool);
        auto highschoolPool = std::make_shared<ContactPool>(4);
        highschool->addPool(highschoolPool);

        auto household = std::make_shared<Household>();
        location->addContactCenter(household);
        auto householdPool = std::make_shared<ContactPool>(5);
        household->addPool(householdPool);

        auto workplace = std::make_shared<Workplace>();
        location->addContactCenter(workplace);
        auto workplacePool = std::make_shared<ContactPool>(6);
        workplace->addPool(workplacePool);

        geoGrid->addLocation(location);
        auto person = std::make_shared<stride::Person>(1, 18, 4, 2, 6, 3, 7, 0, 0, 0, 0, 0);
        communityPool->addMember(person);
        schoolPool->addMember(person);
        secondaryCommunityPool->addMember(person);
        highschoolPool->addMember(person);
        householdPool->addMember(person);
        workplacePool->addMember(person);

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test2.json"));
}
TEST(GeoGridJSONWriterTest, commutesTest)
{
        auto geoGrid   = getGeoGrid();
        auto bavikhove = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        auto gent      = std::make_shared<Location>(2, 4, 2500, Coordinate(0, 0, 0, 0), "Gent");
        auto mons      = std::make_shared<Location>(3, 4, 2500, Coordinate(0, 0, 0, 0), "Mons");

        bavikhove->addOutgoingCommutingLocation(gent, 20);
        bavikhove->addOutgoingCommutingLocation(mons, 1.5);
        gent->addOutgoingCommutingLocation(bavikhove, 42);
        gent->addOutgoingCommutingLocation(mons, 2);

        geoGrid->addLocation(bavikhove);
        geoGrid->addLocation(gent);
        geoGrid->addLocation(mons);

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test7.json"));
}
} // namespace
