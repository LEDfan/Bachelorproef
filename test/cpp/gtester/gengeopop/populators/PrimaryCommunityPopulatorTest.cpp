#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/populators/PrimaryCommunityPopulator.h>
#include <gtest/gtest.h>
#include <util/LogUtils.h>
#include <util/RNManager.h>

using namespace gengeopop;

class PrimaryCommunityPopulatorTest : public testing::Test
{
public:
        PrimaryCommunityPopulatorTest()
            : primaryCommunityPopulator(), rnManager(), config(), location(), primaryCommunity(), geoGrid(), person()
        {
        }

protected:
        virtual void SetUp()
        {
                stride::util::RNManager::Info rnInfo;
                rnInfo.m_seed = 2;
                rnManager     = std::make_shared<stride::util::RNManager>(rnInfo);
                std::shared_ptr<spdlog::logger> logger =
                    stride::util::LogUtils::CreateCliLogger("stride_logger", "stride_log.txt");
                logger->set_level(spdlog::level::off);
                primaryCommunityPopulator = std::make_shared<PrimaryCommunityPopulator>(*rnManager.get(), logger);

                primaryCommunity = std::make_shared<PrimaryCommunity>(1);
                auto pool        = std::make_shared<ContactPool>();
                primaryCommunity->AddPool(pool);

                auto household   = std::make_shared<Household>(2);
                auto contactPool = std::make_shared<ContactPool>();
                person           = std::make_shared<stride::Person>();
                person->SetId(42);
                contactPool->AddMember(person.get());
                household->AddPool(contactPool);
                location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
                location->AddContactCenter(household);
                geoGrid = std::make_shared<GeoGrid>();
                geoGrid->AddLocation(location);
        }

        std::shared_ptr<PrimaryCommunityPopulator> primaryCommunityPopulator;
        std::shared_ptr<stride::util::RNManager>   rnManager;
        GeoGridConfig                              config;
        std::shared_ptr<Location>                  location;
        std::shared_ptr<PrimaryCommunity>          primaryCommunity;
        std::shared_ptr<GeoGrid>                   geoGrid;
        std::shared_ptr<stride::Person>            person;
};

TEST_F(PrimaryCommunityPopulatorTest, OneCommunityTest)
{

        location->AddContactCenter(primaryCommunity);
        geoGrid->Finalize();

        primaryCommunityPopulator->Apply(geoGrid, config);

        const auto& pools = primaryCommunity->GetPools();
        ASSERT_EQ(pools.size(), 1);
        EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
        EXPECT_EQ((*pools[0]->begin())->GetId(), 42);
}

TEST_F(PrimaryCommunityPopulatorTest, ZeroCommunitiesTest)
{
        geoGrid = std::make_shared<GeoGrid>();
        geoGrid->Finalize();
        EXPECT_NO_THROW(primaryCommunityPopulator->Apply(geoGrid, config));
}

TEST_F(PrimaryCommunityPopulatorTest, TwoLocationsTest)
{
        auto location2 = std::make_shared<Location>(2, 5, 1500, Coordinate(1, 1, 1, 1), "Brussel");
        location2->AddContactCenter(primaryCommunity);
        geoGrid->AddLocation(location2);
        geoGrid->Finalize();

        primaryCommunityPopulator->Apply(geoGrid, config);

        const auto& pools = primaryCommunity->GetPools();
        ASSERT_EQ(pools.size(), 1);
        EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
        EXPECT_EQ((*pools[0]->begin())->GetId(), 42);
}
