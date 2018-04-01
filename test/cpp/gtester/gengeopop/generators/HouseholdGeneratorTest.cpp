#include <gengeopop/Household.h>
#include <gengeopop/generators/HouseholdGenerator.h>
#include <gtest/gtest.h>
#include <util/RNManager.h>

using namespace gengeopop;

namespace {

TEST(HouseholdGeneratorTest, OneLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HouseholdGenerator householdGenerator(rnManager);
        GeoGridConfig      config{};
        config.calculated.households = 4;

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
        geoGrid->addLocation(loc1);

        householdGenerator.apply(geoGrid, config);

        const auto& centersOfLoc1 = loc1->getContactCenters();
        EXPECT_EQ(centersOfLoc1.size(), 4);
}

TEST(HouseholdGeneratorTest, ZeroLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HouseholdGenerator householdGenerator(rnManager);
        GeoGridConfig      config{};
        config.calculated.households = 4;

        auto geoGrid = std::make_shared<GeoGrid>();
        householdGenerator.apply(geoGrid, config);

        EXPECT_EQ(geoGrid->size(), 0);
}

TEST(HouseholdGeneratorTest, FiveLocationsTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HouseholdGenerator householdGenerator(rnManager);
        GeoGridConfig      config{};
        config.calculated.households = 4000;
        config.input.populationSize  = 37542 * 100;

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 10150 * 100, Coordinate(0, 0, 0, 0), "Antwerpen");
        auto loc2    = std::make_shared<Location>(2, 4, 10040 * 100, Coordinate(0, 0, 0, 0), "Vlaams-Brabant");
        auto loc3    = std::make_shared<Location>(3, 4, 7460 * 100, Coordinate(0, 0, 0, 0), "Henegouwen");
        auto loc4    = std::make_shared<Location>(4, 4, 3269 * 100, Coordinate(0, 0, 0, 0), "Limburg");
        auto loc5    = std::make_shared<Location>(5, 4, 4123 * 100, Coordinate(0, 0, 0, 0), "Luxemburg");

        geoGrid->addLocation(loc1);
        geoGrid->addLocation(loc2);
        geoGrid->addLocation(loc3);
        geoGrid->addLocation(loc4);
        geoGrid->addLocation(loc5);

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                loc->setRelativePopulation(static_cast<double>(loc->getPopulation()) /
                                           static_cast<double>(config.input.populationSize));
        }

        householdGenerator.apply(geoGrid, config);

        const auto& centersOfLoc1 = loc1->getContactCenters();
        EXPECT_EQ(centersOfLoc1.size(), 1109);

        const auto& centersOfLoc2 = loc2->getContactCenters();
        EXPECT_EQ(centersOfLoc2.size(), 1166);

        const auto& centersOfLoc3 = loc3->getContactCenters();
        EXPECT_EQ(centersOfLoc3.size(), 865);

        const auto& centersOfLoc4 = loc4->getContactCenters();
        EXPECT_EQ(centersOfLoc4.size(), 373);

        const auto& centersOfLoc5 = loc5->getContactCenters();
        EXPECT_EQ(centersOfLoc5.size(), 487);
}

} // namespace
