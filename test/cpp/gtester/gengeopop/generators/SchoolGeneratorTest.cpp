#include <gtest/gtest.h>
#include <util/RNManager.h>
#include <gengeopop/generators/SchoolGenerator.h>
#include <gengeopop/School.h>

using namespace gengeopop;

namespace {

        TEST(SchoolGeneratorTest, OneLocationTest) {

                stride::util::RNManager::Info rnInfo;
                rnInfo.m_seed = 2;
                stride::util::RNManager rnManager(rnInfo);

                SchoolGenerator schoolGenerator(rnManager);
                GeoGridConfig config;
                config.populationSize = 10000;
                config.fraction_compulsoryPupils = 0.20;

                auto geoGrid = std::make_shared<GeoGrid>();
                auto loc1 = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
                geoGrid->addLocation(loc1);

                schoolGenerator.apply(geoGrid, config);

                const auto& centersOfLoc1 = loc1->getContactCenters();
                EXPECT_EQ(centersOfLoc1.size(), 4);

        }

        TEST(SchoolGeneratorTest, ZeroLocationTest) {

                stride::util::RNManager::Info rnInfo;
                rnInfo.m_seed = 2;
                stride::util::RNManager rnManager(rnInfo);

                SchoolGenerator schoolGenerator(rnManager);
                GeoGridConfig config;
                config.populationSize = 10000;
                config.fraction_compulsoryPupils = 0.20;

                auto geoGrid = std::make_shared<GeoGrid>();
                schoolGenerator.apply(geoGrid, config);

                EXPECT_EQ(geoGrid->size(), 0);
        }

        TEST(SchoolGeneratorTest, FiveLocationsTest) {

                stride::util::RNManager::Info rnInfo;
                rnInfo.m_seed = 2;
                stride::util::RNManager rnManager(rnInfo);

                SchoolGenerator schoolGenerator(rnManager);
                GeoGridConfig config;
                config.populationSize = 37542*100; // +- 7500 compulsory pupils -> 15 schools
                config.fraction_compulsoryPupils = 0.20;

                auto geoGrid = std::make_shared<GeoGrid>();
                auto loc1 = std::make_shared<Location>(1, 4, 10150*100, Coordinate(0, 0, 0, 0), "Antwerpen");
                auto loc2 = std::make_shared<Location>(1, 4, 10040*100, Coordinate(0, 0, 0, 0), "Vlaams-Brabant");
                auto loc3 = std::make_shared<Location>(1, 4, 7460*100, Coordinate(0, 0, 0, 0), "Henegouwen");
                auto loc4 = std::make_shared<Location>(1, 4, 3269*100, Coordinate(0, 0, 0, 0), "Limburg");
                auto loc5 = std::make_shared<Location>(1, 4, 4123*100, Coordinate(0, 0, 0, 0), "Luxemburg");

                geoGrid->addLocation(loc1);
                geoGrid->addLocation(loc2);
                geoGrid->addLocation(loc3);
                geoGrid->addLocation(loc4);
                geoGrid->addLocation(loc5);

                schoolGenerator.apply(geoGrid, config);

                const auto& centersOfLoc1 = loc1->getContactCenters();
                EXPECT_EQ(centersOfLoc1.size(), 417);

                const auto& centersOfLoc2 = loc2->getContactCenters();
                EXPECT_EQ(centersOfLoc2.size(), 444);

                const auto& centersOfLoc3 = loc3->getContactCenters();
                EXPECT_EQ(centersOfLoc3.size(), 335);

                const auto& centersOfLoc4 = loc4->getContactCenters();
                EXPECT_EQ(centersOfLoc4.size(), 132);

                const auto& centersOfLoc5 = loc5->getContactCenters();
                EXPECT_EQ(centersOfLoc5.size(), 174);

        }

}  // namespace
