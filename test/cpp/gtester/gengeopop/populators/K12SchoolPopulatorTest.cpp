#include "createGeogrid.h"
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/K12School.h>
#include <gengeopop/populators/K12SchoolPopulator.h>
#include <gtest/gtest.h>
#include <util/LogUtils.h>
#include <util/RNManager.h>

using namespace gengeopop;
using namespace stride;

namespace {

TEST(K12SchoolPopulatorTest, NoPopulation)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        auto geoGrid = std::make_shared<GeoGrid>();

        geoGrid->AddLocation(std::make_shared<Location>(0, 0, 0));

        K12SchoolPopulator k12SchoolPopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));

        GeoGridConfig config{};

        geoGrid->Finalize();

        EXPECT_NO_THROW(k12SchoolPopulator.Apply(geoGrid, config));
}

TEST(K12SchoolPopulatorTest, OneLocationTest)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        K12SchoolPopulator k12SchoolPopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig      config{};

        auto geoGrid = CreateGeoGrid(1, 300, 5, 100, 3);
        geoGrid->Finalize();

        k12SchoolPopulator.Apply(geoGrid, config);

        auto location = *geoGrid->begin();

        auto k12Schools = location->GetContactCentersOfType<K12School>();

        EXPECT_EQ(5, k12Schools.size());

        std::map<int, int> usedCapacity{
            {1, 0},   {2, 1},   {3, 0},   {4, 0},   {5, 0},   {6, 0},   {7, 1},   {8, 1},   {9, 0},   {10, 0},
            {11, 1},  {12, 1},  {13, 0},  {14, 1},  {15, 0},  {16, 2},  {17, 1},  {18, 0},  {19, 1},  {20, 0},
            {21, 0},  {22, 0},  {23, 1},  {24, 1},  {25, 0},  {26, 0},  {27, 0},  {28, 0},  {29, 0},  {30, 0},
            {31, 0},  {32, 1},  {33, 1},  {34, 0},  {35, 0},  {36, 1},  {37, 0},  {38, 1},  {39, 0},  {40, 1},
            {41, 0},  {42, 0},  {43, 1},  {44, 2},  {45, 0},  {46, 1},  {47, 1},  {48, 0},  {49, 1},  {50, 1},
            {51, 0},  {52, 1},  {53, 0},  {54, 0},  {55, 0},  {56, 0},  {57, 0},  {58, 0},  {59, 1},  {60, 0},
            {61, 0},  {62, 0},  {63, 0},  {64, 0},  {65, 2},  {66, 0},  {67, 0},  {68, 0},  {69, 0},  {70, 0},
            {71, 1},  {72, 0},  {73, 0},  {74, 2},  {75, 1},  {76, 0},  {77, 0},  {78, 0},  {79, 0},  {80, 1},
            {81, 0},  {82, 0},  {83, 0},  {84, 0},  {85, 0},  {86, 1},  {87, 0},  {88, 0},  {89, 1},  {90, 0},
            {91, 0},  {92, 0},  {93, 0},  {94, 0},  {95, 0},  {96, 0},  {97, 0},  {98, 0},  {99, 0},  {100, 0},
            {101, 1}, {102, 0}, {103, 0}, {104, 0}, {105, 0}, {106, 0}, {107, 1}, {108, 0}, {109, 0}, {110, 0},
            {111, 1}, {112, 1}, {113, 0}, {114, 0}, {115, 0}, {116, 1}, {117, 1}, {118, 2}, {119, 0}, {120, 2},
            {121, 0}, {122, 0}, {123, 0}, {124, 0}, {125, 0}};

        for (auto& k12School : k12Schools) {
                EXPECT_EQ(25, k12School->GetPools().size());
                for (auto& pool : k12School->GetPools()) {
                        EXPECT_EQ(usedCapacity[pool->GetId()], pool->GetUsedCapacity());
                        for (stride::Person* person : *pool) {
                                EXPECT_LE(person->GetAge(), 18);
                                EXPECT_GE(person->GetAge(), 6);
                        }
                }
        }

        std::map<int, int> persons{
            {0, 118},  {1, 0},    {2, 0},     {3, 0},    {4, 0},     {5, 0},    {6, 0},     {7, 0},     {8, 0},
            {9, 0},    {10, 0},   {11, 47},   {12, 0},   {13, 0},    {14, 71},  {15, 0},    {16, 0},    {17, 0},
            {18, 0},   {19, 0},   {20, 0},    {21, 0},   {22, 0},    {23, 0},   {24, 0},    {25, 0},    {26, 0},
            {27, 0},   {28, 52},  {29, 0},    {30, 0},   {31, 0},    {32, 40},  {33, 0},    {34, 0},    {35, 0},
            {36, 0},   {37, 0},   {38, 0},    {39, 0},   {40, 0},    {41, 0},   {42, 0},    {43, 43},   {44, 0},
            {45, 0},   {46, 0},   {47, 0},    {48, 0},   {49, 118},  {50, 0},   {51, 0},    {52, 0},    {53, 0},
            {54, 0},   {55, 0},   {56, 107},  {57, 0},   {58, 0},    {59, 80},  {60, 32},   {61, 16},   {62, 0},
            {63, 101}, {64, 0},   {65, 0},    {66, 0},   {67, 0},    {68, 38},  {69, 0},    {70, 0},    {71, 0},
            {72, 0},   {73, 0},   {74, 0},    {75, 112}, {76, 0},    {77, 0},   {78, 0},    {79, 0},    {80, 0},
            {81, 0},   {82, 0},   {83, 0},    {84, 0},   {85, 0},    {86, 0},   {87, 116},  {88, 0},    {89, 0},
            {90, 0},   {91, 0},   {92, 0},    {93, 0},   {94, 0},    {95, 0},   {96, 0},    {97, 0},    {98, 0},
            {99, 0},   {100, 0},  {101, 14},  {102, 74}, {103, 0},   {104, 0},  {105, 0},   {106, 0},   {107, 0},
            {108, 24}, {109, 0},  {110, 0},   {111, 0},  {112, 0},   {113, 0},  {114, 0},   {115, 0},   {116, 0},
            {117, 0},  {118, 23}, {119, 0},   {120, 0},  {121, 0},   {122, 0},  {123, 0},   {124, 0},   {125, 0},
            {126, 0},  {127, 0},  {128, 0},   {129, 0},  {130, 0},   {131, 0},  {132, 120}, {133, 0},   {134, 0},
            {135, 0},  {136, 0},  {137, 0},   {138, 0},  {139, 16},  {140, 0},  {141, 0},   {142, 0},   {143, 0},
            {144, 0},  {145, 0},  {146, 0},   {147, 0},  {148, 0},   {149, 0},  {150, 0},   {151, 0},   {152, 0},
            {153, 49}, {154, 0},  {155, 0},   {156, 0},  {157, 120}, {158, 0},  {159, 0},   {160, 0},   {161, 0},
            {162, 0},  {163, 0},  {164, 12},  {165, 0},  {166, 0},   {167, 65}, {168, 0},   {169, 0},   {170, 86},
            {171, 0},  {172, 0},  {173, 33},  {174, 8},  {175, 0},   {176, 0},  {177, 0},   {178, 0},   {179, 0},
            {180, 0},  {181, 0},  {182, 0},   {183, 50}, {184, 0},   {185, 89}, {186, 0},   {187, 0},   {188, 74},
            {189, 0},  {190, 0},  {191, 0},   {192, 65}, {193, 0},   {194, 0},  {195, 0},   {196, 0},   {197, 0},
            {198, 0},  {199, 0},  {200, 0},   {201, 0},  {202, 0},   {203, 0},  {204, 11},  {205, 117}, {206, 0},
            {207, 0},  {208, 7},  {209, 0},   {210, 46}, {211, 44},  {212, 0},  {213, 0},   {214, 0},   {215, 0},
            {216, 0},  {217, 0},  {218, 0},   {219, 0},  {220, 0},   {221, 0},  {222, 0},   {223, 0},   {224, 0},
            {225, 0},  {226, 0},  {227, 0},   {228, 0},  {229, 0},   {230, 19}, {231, 0},   {232, 0},   {233, 0},
            {234, 0},  {235, 0},  {236, 0},   {237, 0},  {238, 0},   {239, 0},  {240, 0},   {241, 0},   {242, 0},
            {243, 0},  {244, 0},  {245, 0},   {246, 0},  {247, 75},  {248, 44}, {249, 0},   {250, 0},   {251, 0},
            {252, 59}, {253, 0},  {254, 0},   {255, 0},  {256, 0},   {257, 36}, {258, 0},   {259, 0},   {260, 0},
            {261, 0},  {262, 0},  {263, 111}, {264, 0},  {265, 0},   {266, 0},  {267, 0},   {268, 0},   {269, 0},
            {270, 0},  {271, 0},  {272, 0},   {273, 0},  {274, 0},   {275, 0},  {276, 0},   {277, 0},   {278, 0},
            {279, 0},  {280, 0},  {281, 0},   {282, 0},  {283, 2},   {284, 0},  {285, 0},   {286, 0},   {287, 0},
            {288, 0},  {289, 0},  {290, 0},   {291, 0},  {292, 0},   {293, 0},  {294, 0},   {295, 0},   {296, 0},
            {297, 17}, {298, 0},  {299, 0}};

        for (const stride::Person& person : *geoGrid->GetPopulation()) {
                EXPECT_EQ(persons[person.GetId()], person.GetK12SchoolId());
        }
}

TEST(K12SchoolPopulatorTest, TwoLocationTest)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        K12SchoolPopulator k12SchoolPopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig      config{};

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3);

        // Brasschaat and Schoten are close to each oter and will both have students from both
        // Kortrijk will only have students going to Kortrijk
        auto brasschaat = *geoGrid->begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));

        auto schoten = *(geoGrid->begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));

        auto kortrijk = *(geoGrid->begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));

        geoGrid->Finalize();

        k12SchoolPopulator.Apply(geoGrid, config);

        auto k12Schools1 = brasschaat->GetContactCentersOfType<K12School>();
        auto k12Schools2 = schoten->GetContactCentersOfType<K12School>();
        auto k12Schools3 = kortrijk->GetContactCentersOfType<K12School>();

        EXPECT_EQ(3, k12Schools1.size());
        EXPECT_EQ(3, k12Schools2.size());
        EXPECT_EQ(3, k12Schools3.size());

        std::map<int, int> persons{
            {0, 66},    {1, 0},     {2, 0},     {3, 0},     {4, 0},     {5, 0},     {6, 0},     {7, 0},     {8, 0},
            {9, 0},     {10, 0},    {11, 164},  {12, 0},    {13, 0},    {14, 10},   {15, 0},    {16, 0},    {17, 0},
            {18, 0},    {19, 0},    {20, 0},    {21, 0},    {22, 0},    {23, 0},    {24, 0},    {25, 0},    {26, 0},
            {27, 0},    {28, 170},  {29, 0},    {30, 0},    {31, 0},    {32, 155},  {33, 0},    {34, 0},    {35, 0},
            {36, 0},    {37, 0},    {38, 0},    {39, 0},    {40, 0},    {41, 0},    {42, 0},    {43, 159},  {44, 0},
            {45, 0},    {46, 0},    {47, 0},    {48, 0},    {49, 66},   {50, 0},    {51, 0},    {52, 0},    {53, 0},
            {54, 0},    {55, 0},    {56, 53},   {57, 0},    {58, 0},    {59, 21},   {60, 146},  {61, 127},  {62, 0},
            {63, 46},   {64, 0},    {65, 0},    {66, 0},    {67, 0},    {68, 154},  {69, 0},    {70, 0},    {71, 0},
            {72, 0},    {73, 0},    {74, 0},    {75, 60},   {76, 0},    {77, 0},    {78, 0},    {79, 0},    {80, 0},
            {81, 0},    {82, 0},    {83, 0},    {84, 0},    {85, 0},    {86, 0},    {87, 65},   {88, 0},    {89, 0},
            {90, 0},    {91, 0},    {92, 0},    {93, 0},    {94, 0},    {95, 0},    {96, 0},    {97, 0},    {98, 0},
            {99, 0},    {100, 0},   {101, 125}, {102, 14},  {103, 0},   {104, 0},   {105, 0},   {106, 0},   {107, 0},
            {108, 137}, {109, 0},   {110, 0},   {111, 0},   {112, 0},   {113, 0},   {114, 0},   {115, 0},   {116, 0},
            {117, 0},   {118, 135}, {119, 0},   {120, 0},   {121, 0},   {122, 0},   {123, 0},   {124, 0},   {125, 0},
            {126, 0},   {127, 0},   {128, 0},   {129, 0},   {130, 0},   {131, 0},   {132, 69},  {133, 0},   {134, 0},
            {135, 0},   {136, 0},   {137, 0},   {138, 0},   {139, 128}, {140, 0},   {141, 0},   {142, 0},   {143, 0},
            {144, 0},   {145, 0},   {146, 0},   {147, 0},   {148, 0},   {149, 0},   {150, 0},   {151, 0},   {152, 0},
            {153, 166}, {154, 0},   {155, 0},   {156, 0},   {157, 68},  {158, 0},   {159, 0},   {160, 0},   {161, 0},
            {162, 0},   {163, 0},   {164, 123}, {165, 0},   {166, 0},   {167, 3},   {168, 0},   {169, 0},   {170, 29},
            {171, 0},   {172, 0},   {173, 147}, {174, 117}, {175, 0},   {176, 0},   {177, 0},   {178, 0},   {179, 0},
            {180, 0},   {181, 0},   {182, 0},   {183, 168}, {184, 0},   {185, 32},  {186, 0},   {187, 0},   {188, 14},
            {189, 0},   {190, 0},   {191, 0},   {192, 3},   {193, 0},   {194, 0},   {195, 0},   {196, 0},   {197, 0},
            {198, 0},   {199, 0},   {200, 0},   {201, 0},   {202, 0},   {203, 0},   {204, 223}, {205, 287}, {206, 0},
            {207, 0},   {208, 221}, {209, 0},   {210, 244}, {211, 243}, {212, 0},   {213, 0},   {214, 0},   {215, 0},
            {216, 0},   {217, 0},   {218, 0},   {219, 0},   {220, 0},   {221, 0},   {222, 0},   {223, 0},   {224, 0},
            {225, 0},   {226, 0},   {227, 0},   {228, 0},   {229, 0},   {230, 228}, {231, 0},   {232, 0},   {233, 0},
            {234, 0},   {235, 0},   {236, 0},   {237, 0},   {238, 0},   {239, 0},   {240, 0},   {241, 0},   {242, 0},
            {243, 0},   {244, 0},   {245, 0},   {246, 0},   {247, 261}, {248, 243}, {249, 0},   {250, 0},   {251, 0},
            {252, 252}, {253, 0},   {254, 0},   {255, 0},   {256, 0},   {257, 238}, {258, 0},   {259, 0},   {260, 0},
            {261, 0},   {262, 0},   {263, 283}, {264, 0},   {265, 0},   {266, 0},   {267, 0},   {268, 0},   {269, 0},
            {270, 0},   {271, 0},   {272, 0},   {273, 0},   {274, 0},   {275, 0},   {276, 0},   {277, 0},   {278, 0},
            {279, 0},   {280, 0},   {281, 0},   {282, 0},   {283, 217}, {284, 0},   {285, 0},   {286, 0},   {287, 0},
            {288, 0},   {289, 0},   {290, 0},   {291, 0},   {292, 0},   {293, 0},   {294, 0},   {295, 0},   {296, 0}};

        for (const stride::Person& person : *geoGrid->GetPopulation()) {
                EXPECT_EQ(persons[person.GetId()], person.GetK12SchoolId());
        }

        for (const auto& household : kortrijk->GetContactCentersOfType<Household>()) {

                for (auto person : *household->GetPools()[0]) {
                        if (person->IsStudentCandidate()) {
                                EXPECT_TRUE(person->GetK12SchoolId() >= 217 && person->GetK12SchoolId() <= 291);
                        } else {
                                EXPECT_EQ(0, person->GetK12SchoolId());
                        }
                }
        }
}

} // namespace
