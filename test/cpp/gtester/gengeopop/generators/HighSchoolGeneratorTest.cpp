#include <gengeopop/HighSchool.h>
#include <gengeopop/generators/HighSchoolGenerator.h>
#include <gengeopop/generators/SchoolGenerator.h>
#include <gtest/gtest.h>
#include <util/RNManager.h>

using namespace gengeopop;

namespace {

TEST(HighSchoolGeneratorTest, OneLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HighSchoolGenerator schoolGenerator(rnManager);
        GeoGridConfig       config{};
        config.input.populationSize               = 45000;
        config.calculated._1826_years_and_student = 9000;

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 45000, Coordinate(0, 0, 0, 0), "Antwerpen");

        geoGrid->addLocation(loc1);

        schoolGenerator.apply(geoGrid, config);

        const auto& centersOfLoc1 = loc1->getContactCenters();
        EXPECT_EQ(centersOfLoc1.size(), 3);
}

TEST(HighSchoolGeneratorTest, ZeroLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HighSchoolGenerator schoolGenerator(rnManager);
        GeoGridConfig       config{};
        config.input.populationSize               = 10000;
        config.calculated._1826_years_and_student = 2000;

        auto geoGrid = std::make_shared<GeoGrid>();
        schoolGenerator.apply(geoGrid, config);

        EXPECT_EQ(geoGrid->size(), 0);
}

TEST(HighSchoolGeneratorTest, FiveLocationsTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HighSchoolGenerator schoolGenerator(rnManager);
        GeoGridConfig       config{};
        config.input.populationSize               = 399992;
        config.calculated._1826_years_and_student = 79998;

        auto             geoGrid = std::make_shared<GeoGrid>();
        std::vector<int> sizes{28559, 33319, 39323, 37755, 35050, 10060, 13468, 8384,
                               9033,  31426, 33860, 4110,  50412, 25098, 40135};
        for (int size : sizes) {
                geoGrid->addLocation(
                    std::make_shared<Location>(1, 4, size, Coordinate(0, 0, 0, 0), "Size: " + std::to_string(size)));
        }

        schoolGenerator.apply(geoGrid, config);

        std::vector<int> expectedSchoolCount{4, 4, 2, 2, 1, 0, 0, 0, 0, 2, 4, 0, 6, 0, 2};
        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(expectedSchoolCount[i], geoGrid->get(i)->getContactCenters().size());
        }
}

} // namespace
