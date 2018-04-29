#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/generators/WorkplaceGenerator.h>
#include <gtest/gtest.h>
#include <tuple>
#include <util/RNManager.h>

#include "../../createlogger.h"
using namespace gengeopop;

namespace {

TEST(WorkplaceGeneratorTest, ZeroLocationTest)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager, CreateLogger());
        GeoGridConfig      config{};
        config.input.populationSize               = 10000;
        config.calculated._1826_years_and_student = 20000;

        auto geoGrid = std::make_shared<GeoGrid>();
        workplaceGenerator.Apply(geoGrid, config);

        EXPECT_EQ(geoGrid->size(), 0);
}

TEST(WorkplaceGeneratorTest, NoCommuting)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager, CreateLogger());
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
                geoGrid->AddLocation(
                    std::make_shared<Location>(1, 4, size, Coordinate(0, 0, 0, 0), "Size: " + std::to_string(size)));
        }

        workplaceGenerator.Apply(geoGrid, config);

        std::vector<int> expectedWorkplaceCount{
            1289, 482,  1849, 1734, 1890, 1012, 1338, 5,    1147, 1679, 651, 468,  464, 1678, 1919, 1343, 852,
            422,  1262, 1702, 1718, 153,  204,  1446, 1811, 1260, 1432, 200, 1535, 890, 494,  1803, 1160, 296,
            1179, 1510, 1024, 615,  777,  920,  1304, 579,  105,  1232, 159, 87,   126, 511,  636,  1648};

        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(expectedWorkplaceCount[i], geoGrid->Get(i)->GetContactCenters().size());
        }
}

TEST(WorkplaceGeneratorTest, AbsNullCommuting)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager, CreateLogger());
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
                geoGrid->AddLocation(
                    std::make_shared<Location>(1, 4, size, Coordinate(0, 0, 0, 0), "Size: " + std::to_string(size)));
        }

        // 10% of the pop of loc0 is commuting to loc1 = 12.833 people
        geoGrid->Get(0)->AddOutgoingCommutingLocation(geoGrid->Get(1), 0.10); // 0.10 is relative to loc0
        geoGrid->Get(1)->AddIncomingCommutingLocation(geoGrid->Get(0), 0.10); // 0.10 is relative to loc0

        // 25,26% of the pop of loc1 is commuting to loc0 = 12.833 people
        geoGrid->Get(1)->AddOutgoingCommutingLocation(geoGrid->Get(0), 0.252697700063012); // 0.25 is relative to loc1
        geoGrid->Get(0)->AddIncomingCommutingLocation(geoGrid->Get(1), 0.252697700063012); // 0.25 is relative to loc1

        EXPECT_EQ(1283, geoGrid->Get(0)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1283, geoGrid->Get(0)->OutGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1283, geoGrid->Get(1)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1283, geoGrid->Get(1)->OutGoingCommutingPeople(config.input.fraction_active_commutingPeople));

        // -> shouldn't change the test outcome in comparision with the previous test

        workplaceGenerator.Apply(geoGrid, config);

        std::vector<int> expectedWorkplaceCount{
            1295, 498,  1852, 1730, 1890, 1010, 1335, 6,    1143, 1684, 652, 467,  462, 1675, 1917, 1349, 851,
            415,  1261, 1707, 1712, 160,  201,  1444, 1811, 1260, 1434, 198, 1536, 889, 492,  1805, 1156, 296,
            1177, 1509, 1025, 619,  772,  923,  1300, 582,  103,  1232, 158, 87,   127, 510,  635,  1648};
        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(expectedWorkplaceCount[i], geoGrid->Get(i)->GetContactCenters().size());
        }
}

TEST(WorkplaceGeneratorTest, TenCommuting)
{

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplaceGenerator workplaceGenerator(rnManager, CreateLogger());
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
                geoGrid->AddLocation(
                    std::make_shared<Location>(1, 4, size, Coordinate(0, 0, 0, 0), "Size: " + std::to_string(size)));
        }

        std::vector<std::tuple<size_t, size_t, double>> commuting{
            // loc0 to loc10 -> 0.23 of loc0
            {0, 10, 0.23}, {25, 3, 0.43}, {38, 17, 0.65}, {10, 17, 0.25}, {15, 17, 0.22}, {27, 17, 0.47},
        };

        for (const std::tuple<size_t, size_t, double>& info : commuting) {
                geoGrid->Get(std::get<0>(info))
                    ->AddOutgoingCommutingLocation(geoGrid->Get(std::get<1>(info)), std::get<2>(info));
                geoGrid->Get(std::get<1>(info))
                    ->AddIncomingCommutingLocation(geoGrid->Get(std::get<0>(info)), std::get<2>(info));
        }

        EXPECT_EQ(2951, geoGrid->Get(0)->OutGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,23 * 128331 * 0,10 = 2951,613
        EXPECT_EQ(0, geoGrid->Get(0)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(1591, geoGrid->Get(10)->OutGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,25 * 63673 * 0,10 = 1591,825
        EXPECT_EQ(2951, geoGrid->Get(10)->IncomingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,23 * 128331 * 0,10 = 2951,613
        EXPECT_EQ(5029, geoGrid->Get(25)->OutGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,43 * 116959 * 0,10 = 5029,023
        EXPECT_EQ(0, geoGrid->Get(25)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(0, geoGrid->Get(3)->OutGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(5029, geoGrid->Get(3)->IncomingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,43 * 116959 * 0,10 = 5029,023
        EXPECT_EQ(0, geoGrid->Get(17)->OutGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(
            10680,
            geoGrid->Get(17)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople)); // = 0,10 * (0,65
                                                                                                      // * 76946 + 0,22
                                                                                                      // * 141389 + 0,47
                                                                                                      // * 20775 + 0,25*
                                                                                                      // 63673) =
                                                                                                      // 10680,298
        EXPECT_EQ(5001, geoGrid->Get(38)->OutGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // = 0,65 * 76946 * 0,10 = 5001,048
        EXPECT_EQ(0, geoGrid->Get(38)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(3110, geoGrid->Get(15)->OutGoingCommutingPeople(
                            config.input.fraction_active_commutingPeople)); // 0,22 * 141389 * 0,10 = 3310,558
        EXPECT_EQ(0, geoGrid->Get(15)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(0, geoGrid->Get(17)->OutGoingCommutingPeople(config.input.fraction_active_commutingPeople));
        EXPECT_EQ(
            10680,
            geoGrid->Get(17)->IncomingCommutingPeople(config.input.fraction_active_commutingPeople)); // = 0,10 * (0,65
                                                                                                      // * 76946 + 0,22
                                                                                                      // * 141389 + 0,47
                                                                                                      // * 20775 + 0,25*
                                                                                                      // 63673) =
                                                                                                      // 10680,298

        workplaceGenerator.Apply(geoGrid, config);

        std::vector<int> expectedWorkplaceCount{
            1282, 481,  1834, 1784, 1884, 1013, 1331, 6,    1142, 1677, 678, 468,  461, 1670, 1908, 1336, 851,
            519,  1273, 1689, 1704, 154,  217,  1425, 1827, 1233, 1435, 205, 1522, 883, 497,  1799, 1160, 299,
            1172, 1492, 1029, 615,  759,  918,  1299, 581,  101,  1231, 158, 84,   124, 514,  639,  1637};
        ;
        for (size_t i = 0; i < sizes.size(); i++) {
                EXPECT_EQ(expectedWorkplaceCount[i], geoGrid->Get(i)->GetContactCenters().size());
        }

        std::cout << std::endl;
}

} // namespace
