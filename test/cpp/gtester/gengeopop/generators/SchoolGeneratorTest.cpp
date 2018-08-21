#include "../../createlogger.h"
#include <gengeopop/K12School.h>
#include <gengeopop/generators/K12SchoolGenerator.h>
#include <gtest/gtest.h>
#include <util/RnMan.h>

using namespace gengeopop;

namespace {

TEST(SchoolGeneratorTest, OneLocationTest)
{

        stride::util::RnMan::Info rnInfo;
        rnInfo.m_seed_seq_init = "1,2,3,4";
        stride::util::RnMan rnManager(rnInfo);

        K12SchoolGenerator schoolGenerator(rnManager, CreateLogger());
        GeoGridConfig      config{};
        config.input.populationSize        = 10000;
        config.calculated.compulsoryPupils = 2000;

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0), "Antwerpen");
        geoGrid->AddLocation(loc1);

        schoolGenerator.Apply(geoGrid, config);

        const auto& centersOfLoc1 = loc1->GetContactCenters();
        EXPECT_EQ(centersOfLoc1.size(), 4);
}

TEST(SchoolGeneratorTest, ZeroLocationTest)
{

        stride::util::RnMan::Info rnInfo;
        rnInfo.m_seed_seq_init = "1,2,3,4";
        stride::util::RnMan rnManager(rnInfo);

        K12SchoolGenerator schoolGenerator(rnManager, CreateLogger());
        GeoGridConfig      config{};
        config.input.populationSize        = 10000;
        config.calculated.compulsoryPupils = 2000;

        auto geoGrid = std::make_shared<GeoGrid>();
        schoolGenerator.Apply(geoGrid, config);

        EXPECT_EQ(geoGrid->size(), 0);
}

TEST(SchoolGeneratorTest, FiveLocationsTest)
{

        stride::util::RnMan::Info rnInfo;
        rnInfo.m_seed_seq_init = "1,2,3,4";
        stride::util::RnMan rnManager(rnInfo);

        K12SchoolGenerator schoolGenerator(rnManager, CreateLogger());
        GeoGridConfig      config{};
        config.input.populationSize        = 37542 * 100;
        config.calculated.compulsoryPupils = 750840;

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 10150 * 100, Coordinate(0, 0), "Antwerpen");
        auto loc2    = std::make_shared<Location>(1, 4, 10040 * 100, Coordinate(0, 0), "Vlaams-Brabant");
        auto loc3    = std::make_shared<Location>(1, 4, 7460 * 100, Coordinate(0, 0), "Henegouwen");
        auto loc4    = std::make_shared<Location>(1, 4, 3269 * 100, Coordinate(0, 0), "Limburg");
        auto loc5    = std::make_shared<Location>(1, 4, 4123 * 100, Coordinate(0, 0), "Luxemburg");

        geoGrid->AddLocation(loc1);
        geoGrid->AddLocation(loc2);
        geoGrid->AddLocation(loc3);
        geoGrid->AddLocation(loc4);
        geoGrid->AddLocation(loc5);

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                loc->SetRelativePopulation(static_cast<double>(loc->GetPopulation()) /
                                           static_cast<double>(config.input.populationSize));
        }

        schoolGenerator.Apply(geoGrid, config);

        const auto& centersOfLoc1 = loc1->GetContactCenters();
        EXPECT_EQ(centersOfLoc1.size(), 417);

        const auto& centersOfLoc2 = loc2->GetContactCenters();
        EXPECT_EQ(centersOfLoc2.size(), 444);

        const auto& centersOfLoc3 = loc3->GetContactCenters();
        EXPECT_EQ(centersOfLoc3.size(), 335);

        const auto& centersOfLoc4 = loc4->GetContactCenters();
        EXPECT_EQ(centersOfLoc4.size(), 132);

        const auto& centersOfLoc5 = loc5->GetContactCenters();
        EXPECT_EQ(centersOfLoc5.size(), 174);
}

} // namespace
