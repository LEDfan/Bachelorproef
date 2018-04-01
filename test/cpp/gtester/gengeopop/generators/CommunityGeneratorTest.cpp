#include <gengeopop/Community.h>
#include <gengeopop/generators/CommunityGenerator.h>
#include <gtest/gtest.h>
#include <util/RNManager.h>

using namespace gengeopop;

namespace {

TEST(CommunityGeneratorTest, OneLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        CommunityGenerator communityGenerator(rnManager);
        GeoGridConfig      config;
        config.input.populationSize = 10000;

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
        geoGrid->addLocation(loc1);

        communityGenerator.apply(geoGrid, config);

        const auto& centersOfLoc1 = loc1->getContactCenters();
        EXPECT_EQ(centersOfLoc1.size(), 5);
}

TEST(CommunityGeneratorTest, EqualLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 10;
        stride::util::RNManager rnManager(rnInfo);

        CommunityGenerator communityGenerator(rnManager);
        GeoGridConfig      config;
        config.input.populationSize = 100 * 100 * 1000;

        auto geoGrid = std::make_shared<GeoGrid>();
        for (int i = 0; i < 10; i++) {
                geoGrid->addLocation(std::make_shared<Location>(1, 4, 10 * 1000 * 1000, Coordinate(0, 0, 0, 0),
                                                                "Location " + std::to_string(i)));
        }

        communityGenerator.apply(geoGrid, config);

        std::vector<int> expectedCount{520, 499, 509, 534, 472, 506, 472, 485, 491, 512};
        for (int i = 0; i < 10; i++) {
                EXPECT_EQ(expectedCount[i], geoGrid->get(i)->getContactCenters().size());
        }
}

TEST(CommunityGeneratorTest, ZeroLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        CommunityGenerator communityGenerator(rnManager);
        GeoGridConfig      config;
        config.input.populationSize = 10000;

        auto geoGrid = std::make_shared<GeoGrid>();
        communityGenerator.apply(geoGrid, config);

        EXPECT_EQ(geoGrid->size(), 0);
}

TEST(CommunityGeneratorTest, FiveLocationsTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        CommunityGenerator communityGenerator(rnManager);
        GeoGridConfig      config{};
        config.input.populationSize        = 37542 * 100;
        config.calculated.compulsoryPupils = 750840;

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 10150 * 100, Coordinate(0, 0, 0, 0), "Antwerpen");
        auto loc2    = std::make_shared<Location>(1, 4, 10040 * 100, Coordinate(0, 0, 0, 0), "Vlaams-Brabant");
        auto loc3    = std::make_shared<Location>(1, 4, 7460 * 100, Coordinate(0, 0, 0, 0), "Henegouwen");
        auto loc4    = std::make_shared<Location>(1, 4, 3269 * 100, Coordinate(0, 0, 0, 0), "Limburg");
        auto loc5    = std::make_shared<Location>(1, 4, 4123 * 100, Coordinate(0, 0, 0, 0), "Luxemburg");

        geoGrid->addLocation(loc1);
        geoGrid->addLocation(loc2);
        geoGrid->addLocation(loc3);
        geoGrid->addLocation(loc4);
        geoGrid->addLocation(loc5);

        communityGenerator.apply(geoGrid, config);

        const auto& centersOfLoc1 = loc1->getContactCenters();
        EXPECT_EQ(centersOfLoc1.size(), 509);

        const auto& centersOfLoc2 = loc2->getContactCenters();
        EXPECT_EQ(centersOfLoc2.size(), 565);

        const auto& centersOfLoc3 = loc3->getContactCenters();
        EXPECT_EQ(centersOfLoc3.size(), 420);

        const auto& centersOfLoc4 = loc4->getContactCenters();
        EXPECT_EQ(centersOfLoc4.size(), 170);

        const auto& centersOfLoc5 = loc5->getContactCenters();
        EXPECT_EQ(centersOfLoc5.size(), 214);
}

} // namespace
