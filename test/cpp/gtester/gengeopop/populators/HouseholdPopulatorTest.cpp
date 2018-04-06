#include <gengeopop/Household.h>
#include <gengeopop/populators/HouseholdPopulator.h>
#include <gtest/gtest.h>
#include <util/RNManager.h>

using namespace gengeopop;

namespace {

// FIXME: use logger instead
void disableCout() { std::cout.setstate(std::ios_base::failbit); }
void enableCout() { std::cout.clear(); }

TEST(HouseholdPopulatorTest, OneHouseholdTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HouseholdPopulator householdPopulator(rnManager);
        GeoGridConfig      config{};
        auto               householdType = std::make_shared<Household>();
        auto               poolType      = std::make_shared<ContactPool>();
        auto               personType    = std::make_shared<stride::Person>();
        personType->SetAge(18);
        poolType->addMember(personType);
        householdType->addPool(poolType);
        config.generated.household_types.push_back(householdType);

        auto geoGrid   = std::make_shared<GeoGrid>();
        auto loc1      = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
        auto household = std::make_shared<Household>();
        household->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household);
        geoGrid->addLocation(loc1);

        disableCout();
        householdPopulator.apply(geoGrid, config);
        enableCout();

        const auto& pools = household->GetPools();
        ASSERT_EQ(pools.size(), 1);
        EXPECT_EQ(pools[0]->getUsedCapacity(), 1);
}

TEST(HouseholdPopulatorTest, ZeroHouseholdsTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HouseholdPopulator householdPopulator(rnManager);
        GeoGridConfig      config{};

        auto geoGrid = std::make_shared<GeoGrid>();

        disableCout();
        EXPECT_NO_THROW(householdPopulator.apply(geoGrid, config));
        enableCout();
}

TEST(HouseholdPopulatorTest, FiveHouseholdsTest)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HouseholdPopulator householdPopulator(rnManager);
        GeoGridConfig      config{};
        auto               householdType = std::make_shared<Household>();
        auto               poolType      = std::make_shared<ContactPool>();
        auto               personType    = std::make_shared<stride::Person>();
        personType->SetAge(18);
        poolType->addMember(personType);
        householdType->addPool(poolType);
        config.generated.household_types.push_back(householdType);

        auto geoGrid = std::make_shared<GeoGrid>();
        auto loc1    = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");

        auto household1 = std::make_shared<Household>();
        household1->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household1);
        auto household2 = std::make_shared<Household>();
        household2->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household2);
        auto household3 = std::make_shared<Household>();
        household3->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household3);
        auto household4 = std::make_shared<Household>();
        household4->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household4);
        auto household5 = std::make_shared<Household>();
        household5->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household5);

        geoGrid->addLocation(loc1);

        disableCout();
        householdPopulator.apply(geoGrid, config);
        enableCout();

        for (auto household : *loc1) {
                ASSERT_EQ(household->GetPools().size(), 1);
                ASSERT_EQ(household->GetPools()[0]->getUsedCapacity(), 1);
                EXPECT_EQ((*household->GetPools()[0]->begin())->GetAge(), 18);
        }
}
TEST(HouseholdPopulatorTest, MultipleHouseholdTypesTest)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        HouseholdPopulator householdPopulator(rnManager);
        GeoGridConfig      config{};
        {
                auto householdType = std::make_shared<Household>();
                auto poolType      = std::make_shared<ContactPool>();
                auto personType    = std::make_shared<stride::Person>();
                personType->SetAge(18);
                poolType->addMember(personType);
                householdType->addPool(poolType);
                config.generated.household_types.push_back(householdType);
        }
        {
                auto householdType = std::make_shared<Household>();
                auto poolType      = std::make_shared<ContactPool>();
                auto personType    = std::make_shared<stride::Person>();
                personType->SetAge(12);
                poolType->addMember(personType);
                householdType->addPool(poolType);
                auto personType2 = std::make_shared<stride::Person>();
                personType2->SetAge(56);
                poolType->addMember(personType2);
                config.generated.household_types.push_back(householdType);
        }

        auto geoGrid   = std::make_shared<GeoGrid>();
        auto loc1      = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Antwerpen");
        auto household = std::make_shared<Household>();
        household->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household);
        geoGrid->addLocation(loc1);
        auto household2 = std::make_shared<Household>();
        household2->addPool(std::make_shared<ContactPool>());
        loc1->addContactCenter(household2);

        disableCout();
        householdPopulator.apply(geoGrid, config);
        enableCout();

        {
                const auto& pools = household->GetPools();
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->getUsedCapacity(), 1);
                EXPECT_EQ((*pools[0]->begin())->GetAge(), 18);
        }
        {
                const auto& pools = household2->GetPools();
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->getUsedCapacity(), 2);
                EXPECT_EQ((*pools[0]->begin())->GetAge(), 12);
                EXPECT_EQ((*(pools[0]->begin() + 1))->GetAge(), 56);
        }
}

} // namespace
