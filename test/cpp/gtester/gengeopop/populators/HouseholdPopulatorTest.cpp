#include <gengeopop/Household.h>
#include <gengeopop/populators/HouseholdPopulator.h>
#include <gtest/gtest.h>
#include <util/LogUtils.h>
#include <util/RNManager.h>

using namespace gengeopop;

class HouseholdPopulatorTest : public testing::Test
{
public:
        HouseholdPopulatorTest() : householdPopulator(), rnManager() {}

protected:
        virtual void SetUp()
        {
                stride::util::RNManager::Info rnInfo;
                rnInfo.m_seed = 2;
                rnManager     = std::make_shared<stride::util::RNManager>(rnInfo);
                std::shared_ptr<spdlog::logger> logger =
                    stride::util::LogUtils::CreateCliLogger("stride_logger", "stride_log.txt");
                logger->set_level(spdlog::level::off);
                householdPopulator = std::make_shared<HouseholdPopulator>(*rnManager.get(), logger);
        }

        std::shared_ptr<HouseholdPopulator>      householdPopulator;
        std::shared_ptr<stride::util::RNManager> rnManager;
};

TEST_F(HouseholdPopulatorTest, OneHouseholdTest)
{
        GeoGridConfig config{};
        auto          householdType = std::make_shared<Household>();
        auto          poolType      = std::make_shared<ContactPool>();
        auto          personType    = std::make_shared<stride::Person>();
        personType->SetAge(18);
        poolType->AddMember(personType.get());
        householdType->AddPool(poolType);
        config.generated.household_types.push_back(householdType);

        auto geoGrid   = std::make_shared<GeoGrid>();
        auto loc1      = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
        auto household = std::make_shared<Household>();
        household->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household);
        geoGrid->AddLocation(loc1);

        householdPopulator->Apply(geoGrid, config);

        const auto& pools = household->GetPools();
        ASSERT_EQ(pools.size(), 1);
        EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
}

TEST_F(HouseholdPopulatorTest, ZeroHouseholdsTest)
{
        GeoGridConfig config{};

        auto geoGrid = std::make_shared<GeoGrid>();

        EXPECT_NO_THROW(householdPopulator->Apply(geoGrid, config));
}

TEST_F(HouseholdPopulatorTest, FiveHouseholdsTest)
{
        GeoGridConfig config{};
        auto          householdType = std::make_shared<Household>();
        auto          poolType      = std::make_shared<ContactPool>();
        auto          personType    = std::make_shared<stride::Person>();
        personType->SetAge(18);
        poolType->AddMember(personType.get());
        householdType->AddPool(poolType);
        config.generated.household_types.push_back(householdType);

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");

        auto household1 = std::make_shared<Household>();
        household1->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household1);
        auto household2 = std::make_shared<Household>();
        household2->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household2);
        auto household3 = std::make_shared<Household>();
        household3->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household3);
        auto household4 = std::make_shared<Household>();
        household4->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household4);
        auto household5 = std::make_shared<Household>();
        household5->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household5);

        geoGrid->AddLocation(loc1);

        householdPopulator->Apply(geoGrid, config);

        for (auto household : *loc1) {
                ASSERT_EQ(household->GetPools().size(), 1);
                ASSERT_EQ(household->GetPools()[0]->GetUsedCapacity(), 1);
                EXPECT_EQ((*household->GetPools()[0]->begin())->GetAge(), 18);
        }
}
TEST_F(HouseholdPopulatorTest, MultipleHouseholdTypesTest)
{
        std::shared_ptr<stride::Person> personType1;
        std::shared_ptr<stride::Person> personType2;
        std::shared_ptr<stride::Person> personType3;

        GeoGridConfig config{};
        {
                auto householdType = std::make_shared<Household>();
                auto poolType      = std::make_shared<ContactPool>();
                personType1        = std::make_shared<stride::Person>();
                personType1->SetAge(18);
                poolType->AddMember(personType1.get());
                householdType->AddPool(poolType);
                config.generated.household_types.push_back(householdType);
        }
        {
                auto householdType = std::make_shared<Household>();
                auto poolType      = std::make_shared<ContactPool>();
                personType2        = std::make_shared<stride::Person>();
                personType2->SetAge(12);
                poolType->AddMember(personType2.get());
                householdType->AddPool(poolType);
                personType3 = std::make_shared<stride::Person>();
                personType3->SetAge(56);
                poolType->AddMember(personType3.get());
                config.generated.household_types.push_back(householdType);
        }

        auto geoGrid   = std::make_shared<GeoGrid>();
        auto loc1      = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
        auto household = std::make_shared<Household>();
        household->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household);
        geoGrid->AddLocation(loc1);
        auto household2 = std::make_shared<Household>();
        household2->AddPool(std::make_shared<ContactPool>());
        loc1->AddContactCenter(household2);
        householdPopulator->Apply(geoGrid, config);

        std::map<int, std::vector<std::shared_ptr<ContactPool>>> pools_map;
        pools_map[household->GetPools()[0]->GetUsedCapacity()]  = household->GetPools();
        pools_map[household2->GetPools()[0]->GetUsedCapacity()] = household2->GetPools();
        {
                const auto& pools = pools_map[1];
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->GetUsedCapacity(), 1);
                EXPECT_EQ((*pools[0]->begin())->GetAge(), 18);
        }
        {
                const auto& pools = pools_map[2];
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->GetUsedCapacity(), 2);
                EXPECT_EQ((*pools[0]->begin())->GetAge(), 12);
                EXPECT_EQ((*(pools[0]->begin() + 1))->GetAge(), 56);
        }
}
