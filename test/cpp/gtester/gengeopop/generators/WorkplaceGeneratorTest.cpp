#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/generators/WorkplaceGenerator.h>
#include <gtest/gtest.h>
#include <tuple>
#include <util/RNManager.h>

using namespace gengeopop;

namespace {

TEST(WorkplaceGeneratorTest, ZeroLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager);
        GeoGridConfig      config{};
        config.input.populationSize               = 10000;
        config.calculated._1826_years_and_student = 20000;

        auto geoGrid = std::make_shared<GeoGrid>();
        workplaceGenerator.apply(geoGrid, config);

        EXPECT_EQ(geoGrid->size(), 0);
}

TEST(WorkplaceGeneratorTest, NoCommuting)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager);
        GeoGridConfig      config{};
        config.input.populationSize                  = 5 * 1000 * 1000;
        config.calculated._1865_and_years_active     = static_cast<unsigned int>(0.20 * 5 * 1000 * 1000);
        config.input.fraction_1865_years_active      = 0.20;
        config.input.fraction_active_commutingPeople = 0;

        auto geoGrid = std::make_shared<GeoGrid>();

        std::vector<int> sizes{128331, 50784,  191020, 174476, 186595, 105032, 136388, 577,   111380, 171014,
                               63673,  49438,  45590,  164666, 185249, 141389, 82525,  40397, 123307, 168128,
                               172937, 15581,  22891,  143505, 178516, 116959, 144659, 20775, 156009, 91951,
                               49823,  181594, 119075, 27700,  116959, 146583, 102531, 58864, 76946,  91951,
                               134464, 59248,  10003,  125423, 15004,  8656,   13658,  50784, 61749,  165243};
        for (int size : sizes) {
                geoGrid->addLocation(
                    std::make_shared<Location>(1, 4, size, Coordinate(0, 0, 0, 0), "Size: " + std::to_string(size)));
        }

        workplaceGenerator.apply(geoGrid, config);

        std::vector<int> expectedWorkplaceCount{
            1289, 482,  1849, 1734, 1890, 1012, 1338, 5,    1147, 1679, 651, 468,  464, 1678, 1919, 1343, 852,
            422,  1262, 1702, 1718, 153,  204,  1446, 1811, 1260, 1432, 200, 1535, 890, 494,  1803, 1160, 296,
            1179, 1510, 1024, 615,  777,  920,  1304, 579,  105,  1232, 159, 87,   126, 511,  636,  1648};

        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(expectedWorkplaceCount[i], geoGrid->get(i)->getContactCenters().size());
        }
}

TEST(WorkplaceGeneratorTest, AbsNullCommuting)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager);
        GeoGridConfig      config{};
        config.input.populationSize                  = 5 * 1000 * 1000;
        config.calculated._1865_and_years_active     = static_cast<unsigned int>(0.20 * 5 * 1000 * 1000);
        config.input.fraction_1865_years_active      = 0.20;
        config.input.fraction_active_commutingPeople = 0.10;

        auto geoGrid = std::make_shared<GeoGrid>();

        std::vector<int> sizes{128331, 50784,  191020, 174476, 186595, 105032, 136388, 577,   111380, 171014,
                               63673,  49438,  45590,  164666, 185249, 141389, 82525,  40397, 123307, 168128,
                               172937, 15581,  22891,  143505, 178516, 116959, 144659, 20775, 156009, 91951,
                               49823,  181594, 119075, 27700,  116959, 146583, 102531, 58864, 76946,  91951,
                               134464, 59248,  10003,  125423, 15004,  8656,   13658,  50784, 61749,  165243};
        for (int size : sizes) {
                geoGrid->addLocation(
                    std::make_shared<Location>(1, 4, size, Coordinate(0, 0, 0, 0), "Size: " + std::to_string(size)));
        }

        // 10% of the pop of loc0 is commuting to loc1 = 12.833 people
        geoGrid->get(0)->addOutgoingCommutingLocation(geoGrid->get(1), 0.10); // 0.10 is relative to loc0
        geoGrid->get(1)->addIncomingCommutingLocation(geoGrid->get(0), 0.10); // 0.10 is relative to loc0

        // 25,26% of the pop of loc1 is commuting to loc0 = 12.833 people
        geoGrid->get(1)->addOutgoingCommutingLocation(geoGrid->get(0), 0.252697700063012); // 0.25 is relative to loc1
        geoGrid->get(0)->addIncomingCommutingLocation(geoGrid->get(1), 0.252697700063012); // 0.25 is relative to loc1

        EXPECT_EQ(1283, geoGrid->get(0)->incomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1283, geoGrid->get(0)->outGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1283, geoGrid->get(1)->incomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1283, geoGrid->get(1)->outGoingCommutingPeople(config.input.fraction_active_commutingPeople));

        // -> shouldn't change the test outcome in comparision with the previous test

        workplaceGenerator.apply(geoGrid, config);

        std::vector<int> expectedWorkplaceCount{
            1289, 482,  1849, 1734, 1890, 1012, 1338, 5,    1147, 1679, 651, 468,  464, 1678, 1919, 1343, 852,
            422,  1262, 1702, 1718, 153,  204,  1446, 1811, 1260, 1432, 200, 1535, 890, 494,  1803, 1160, 296,
            1179, 1510, 1024, 615,  777,  920,  1304, 579,  105,  1232, 159, 87,   126, 511,  636,  1648};
        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(expectedWorkplaceCount[i], geoGrid->get(i)->getContactCenters().size());
        }
}

TEST(WorkplaceGeneratorTest, TenCommuting)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager);
        GeoGridConfig      config{};
        config.input.populationSize                  = 5 * 1000 * 1000;
        config.calculated._1865_and_years_active     = static_cast<unsigned int>(0.20 * 5 * 1000 * 1000);
        config.input.fraction_1865_years_active      = 0.20;
        config.input.fraction_active_commutingPeople = 0.10;

        auto geoGrid = std::make_shared<GeoGrid>();

        std::vector<int> sizes{128331, 50784,  191020, 174476, 186595, 105032, 136388, 577,   111380, 171014,
                               63673,  49438,  45590,  164666, 185249, 141389, 82525,  40397, 123307, 168128,
                               172937, 15581,  22891,  143505, 178516, 116959, 144659, 20775, 156009, 91951,
                               49823,  181594, 119075, 27700,  116959, 146583, 102531, 58864, 76946,  91951,
                               134464, 59248,  10003,  125423, 15004,  8656,   13658,  50784, 61749,  165243};
        for (int size : sizes) {
                geoGrid->addLocation(
                    std::make_shared<Location>(1, 4, size, Coordinate(0, 0, 0, 0), "Size: " + std::to_string(size)));
        }

        std::vector<std::tuple<size_t, size_t, double>> commuting{
            // loc0 to loc10 -> 0.23 of loc0
            {0, 10, 0.23}, {25, 3, 0.43}, {38, 17, 0.65}, {10, 17, 0.25}, {15, 17, 0.22}, {27, 17, 0.47},
        };

        for (const std::tuple<size_t, size_t, double>& info : commuting) {
                geoGrid->get(std::get<0>(info))
                    ->addOutgoingCommutingLocation(geoGrid->get(std::get<1>(info)), std::get<2>(info));
                geoGrid->get(std::get<1>(info))
                    ->addIncomingCommutingLocation(geoGrid->get(std::get<0>(info)), std::get<2>(info));
        }

        EXPECT_EQ(2951, geoGrid->get(0)->outGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,23 * 128331 * 0,10 = 2951,613
        EXPECT_EQ(0, geoGrid->get(0)->incomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1591, geoGrid->get(10)->outGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,25 * 63673 * 0,10 = 1591,825
        EXPECT_EQ(2951, geoGrid->get(10)->incomingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,23 * 128331 * 0,10 = 2951,613
        EXPECT_EQ(5029, geoGrid->get(25)->outGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,43 * 116959 * 0,10 = 5029,023
        EXPECT_EQ(0, geoGrid->get(25)->incomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(0, geoGrid->get(3)->outGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(5029, geoGrid->get(3)->incomingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,43 * 116959 * 0,10 = 5029,023
        EXPECT_EQ(0, geoGrid->get(17)->outGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(
            10680,
            geoGrid->get(17)->incomingCommutingPeople(config.input.fraction_active_commutingPeople)); // = 0,10 * (0,65
                                                                                                      // * 76946 + 0,22
                                                                                                      // * 141389 + 0,47
                                                                                                      // * 20775 + 0,25*
                                                                                                      // 63673) =
                                                                                                      // 10680,298
        EXPECT_EQ(5001, geoGrid->get(38)->outGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,65 * 76946 * 0,10 = 5001,048
        EXPECT_EQ(0, geoGrid->get(38)->incomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(3110, geoGrid->get(15)->outGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // 0,22 * 141389 * 0,10 = 3310,558
        EXPECT_EQ(0, geoGrid->get(15)->incomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(0, geoGrid->get(17)->outGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(
            10680,
            geoGrid->get(17)->incomingCommutingPeople(config.input.fraction_active_commutingPeople)); // = 0,10 * (0,65
                                                                                                      // * 76946 + 0,22
                                                                                                      // * 141389 + 0,47
                                                                                                      // * 20775 + 0,25*
                                                                                                      // 63673) =
                                                                                                      // 10680,298

        workplaceGenerator.apply(geoGrid, config);

        std::vector<int> expectedWorkplaceCount{
            1133, 511,  1829, 1987, 1870, 1020, 1350, 7,    1127, 1696, 735, 467,  475, 1647, 1898, 1215, 855,
            933,  1271, 1679, 1734, 175,  216,  1430, 1851, 970,  1423, 144, 1528, 895, 501,  1796, 1184, 306,
            1179, 1500, 1023, 606,  527,  920,  1304, 579,  105,  1232, 159, 87,   126, 511,  636,  1648};
        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(expectedWorkplaceCount[i], geoGrid->get(i)->getContactCenters().size());
        }

        std::cout << std::endl;
}

} // namespace
