#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/SecondaryCommunity.h>
#include <gengeopop/populators/PrimaryCommunityPopulator.h>
#include <gengeopop/populators/SecondaryCommunityPopulator.h>
#include <gtest/gtest.h>
#include <util/LogUtils.h>
#include <util/RNManager.h>

using namespace gengeopop;

template <typename CommunityType>
class CommunityPopulatorTest : public testing::Test
{
public:
        CommunityPopulatorTest()
            : communityPopulator(), rnManager(), config(), location(), community(), geoGrid(), person(), logger()
        {
        }

protected:
        virtual void SetUp()
        {
                stride::util::RNManager::Info rnInfo;
                rnInfo.m_seed = 5;
                rnManager     = std::make_shared<stride::util::RNManager>(rnInfo);
                logger        = stride::util::LogUtils::CreateCliLogger("stride_logger", "stride_log.txt");
                logger->set_level(spdlog::level::off);

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

                community = std::make_shared<CommunityType>(1);
                auto pool = std::make_shared<ContactPool>();
                community->AddPool(pool);
        }

        void OneCommunityTest()
        {
                location->AddContactCenter(community);
                geoGrid->Finalize();

                communityPopulator->Apply(geoGrid, config);

                const auto& pools = community->GetPools();
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
                EXPECT_EQ((*pools[0]->begin())->GetId(), 42);
        }

        void ZeroCommunitiesTest()
        {
                geoGrid = std::make_shared<GeoGrid>();
                geoGrid->Finalize();
                EXPECT_NO_THROW(communityPopulator->Apply(geoGrid, config));
        }

        void EmptyLocationTest()
        {
                geoGrid  = std::make_shared<GeoGrid>();
                location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
                location->AddContactCenter(community);
                geoGrid->AddLocation(location);
                geoGrid->Finalize();
                EXPECT_NO_THROW(communityPopulator->Apply(geoGrid, config));
        }

        void TwoLocationsTest()
        {
                location->AddContactCenter(community);

                auto location2 = std::make_shared<Location>(2, 5, 1500, Coordinate(1, 1, 1, 1), "Brussel");

                auto community2 = std::make_shared<PrimaryCommunity>(1);
                auto pool       = std::make_shared<ContactPool>();
                community2->AddPool(pool);
                location2->AddContactCenter(community2);

                geoGrid->AddLocation(location2);
                geoGrid->Finalize();

                communityPopulator->Apply(geoGrid, config);

                {
                        const auto& pools = community->GetPools();
                        ASSERT_EQ(pools.size(), 1);
                        EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
                        EXPECT_EQ((*pools[0]->begin())->GetId(), 42);
                }
                {
                        const auto& pools = community2->GetPools();
                        ASSERT_EQ(pools.size(), 1);
                        EXPECT_EQ(pools[0]->GetUsedCapacity(), 0);
                }
        }

        void OtherLocationTest()
        {
                auto location2 = std::make_shared<Location>(2, 5, 1500, Coordinate(1, 1, 1, 1), "Brussel");
                location2->AddContactCenter(community);
                geoGrid->AddLocation(location2);
                geoGrid->Finalize();

                communityPopulator->Apply(geoGrid, config);

                const auto& pools = community->GetPools();
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
                EXPECT_EQ((*pools[0]->begin())->GetId(), 42);
        }

        void HouseholdTest()
        {
                auto pool    = *location->GetContactCentersOfType<Household>()[0]->begin();
                auto person2 = std::make_shared<stride::Person>();
                person2->SetId(5);
                person2->SetAge(2);
                pool->AddMember(person2.get());
                location->AddContactCenter(community);

                auto community2 = std::make_shared<CommunityType>(2);

                {
                        auto pool = std::make_shared<ContactPool>();
                        community2->AddPool(pool);
                        location->AddContactCenter(community2);
                }

                geoGrid->Finalize();

                communityPopulator->Apply(geoGrid, config);

                HouseholdTestCheck(community2);
        }

        virtual void HouseholdTestCheck(std::shared_ptr<Community> community2) = 0;

        std::shared_ptr<PartialPopulator>        communityPopulator;
        std::shared_ptr<stride::util::RNManager> rnManager;
        GeoGridConfig                            config;
        std::shared_ptr<Location>                location;
        std::shared_ptr<CommunityType>           community;
        std::shared_ptr<GeoGrid>                 geoGrid;
        std::shared_ptr<stride::Person>          person;
        std::shared_ptr<spdlog::logger>          logger;
};

class PrimaryCommunityPopulatorTest : public CommunityPopulatorTest<PrimaryCommunity>
{
public:
        PrimaryCommunityPopulatorTest() {}

protected:
        void SetUp() override
        {
                CommunityPopulatorTest::SetUp();
                communityPopulator = std::make_shared<PrimaryCommunityPopulator>(*rnManager.get(), logger);
        }

        void HouseholdTestCheck(std::shared_ptr<Community> community2) override
        {
                {
                        const auto& pools = community->GetPools();
                        ASSERT_EQ(pools.size(), 1);
                        EXPECT_EQ(pools[0]->GetUsedCapacity(), 2);
                        EXPECT_EQ((*pools[0]->begin())->GetId(), 42);
                        EXPECT_EQ((*(pools[0]->begin() + 1))->GetId(), 5);
                }
                {
                        const auto& pools = community2->GetPools();
                        ASSERT_EQ(pools.size(), 1);
                        EXPECT_EQ(pools[0]->GetUsedCapacity(), 0);
                }
        }
};

class SecondaryCommunityPopulatorTest : public CommunityPopulatorTest<SecondaryCommunity>
{
public:
        SecondaryCommunityPopulatorTest() {}

protected:
        void SetUp() override
        {
                CommunityPopulatorTest::SetUp();
                communityPopulator = std::make_shared<SecondaryCommunityPopulator>(*rnManager.get(), logger);
        }

        void HouseholdTestCheck(std::shared_ptr<Community> community2) override
        {
                {
                        const auto& pools = community->GetPools();
                        ASSERT_EQ(pools.size(), 1);
                        EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
                        EXPECT_EQ((*pools[0]->begin())->GetId(), 42);
                }
                {
                        const auto& pools = community2->GetPools();
                        ASSERT_EQ(pools.size(), 1);
                        EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
                        EXPECT_EQ((*pools[0]->begin())->GetId(), 5);
                }
        }
};

TEST_F(PrimaryCommunityPopulatorTest, OneCommunityTest) { OneCommunityTest(); }

TEST_F(PrimaryCommunityPopulatorTest, ZeroCommunitiesTest) { ZeroCommunitiesTest(); }

TEST_F(PrimaryCommunityPopulatorTest, EmptyLocationTest) { EmptyLocationTest(); }

TEST_F(PrimaryCommunityPopulatorTest, HouseholdTest) { HouseholdTest(); }

TEST_F(PrimaryCommunityPopulatorTest, TwoLocationsTest) { TwoLocationsTest(); }

TEST_F(PrimaryCommunityPopulatorTest, OtherLocationTest) { OtherLocationTest(); }

TEST_F(SecondaryCommunityPopulatorTest, OneCommunityTest) { OneCommunityTest(); }

TEST_F(SecondaryCommunityPopulatorTest, ZeroCommunitiesTest) { ZeroCommunitiesTest(); }

TEST_F(SecondaryCommunityPopulatorTest, EmptyLocationTest) { EmptyLocationTest(); }

TEST_F(SecondaryCommunityPopulatorTest, HouseholdTest) { HouseholdTest(); }

TEST_F(SecondaryCommunityPopulatorTest, TwoLocationsTest) { TwoLocationsTest(); }

TEST_F(SecondaryCommunityPopulatorTest, OtherLocationTest) { OtherLocationTest(); }
