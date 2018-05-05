#include "createGeogrid.h"
#include <gengeopop/College.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/K12School.h>
#include <gengeopop/populators/CollegePopulator.h>
#include <gtest/gtest.h>
#include <util/LogUtils.h>
#include <util/RNManager.h>

using namespace gengeopop;
using namespace stride;

namespace {

TEST(CollegePopulatorTest, NoPopulation)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        auto geoGrid = std::make_shared<GeoGrid>();

        geoGrid->AddLocation(std::make_shared<Location>(0, 0, 0));

        CollegePopulator collegePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));

        GeoGridConfig config{};

        geoGrid->Finalize();

        EXPECT_NO_THROW(collegePopulator.Apply(geoGrid, config));
}

TEST(CollegePopulatorTest, NoStudents)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        CollegePopulator collegePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig    config{};
        config.input.fraction_student_commutingPeople     = 0;
        config.input.fraction_1826_years_WhichAreStudents = 0;

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3, config);

        auto location = *geoGrid->begin();

        // Brasschaat and Schoten are close to each other
        // There is no commuting, but since they will still receive students from each other
        // Kortrijk will only receive students from Kortrijik
        auto brasschaat = *geoGrid->begin();
        brasschaat->SetCoordinate(Coordinate(0, 0, 51.29227, 4.49419));
        auto collegeBra = std::make_shared<College>(config.generated.contactCenters++);
        collegeBra->Fill(config);
        brasschaat->AddContactCenter(collegeBra);

        auto schoten = *(geoGrid->begin() + 1);
        schoten->SetCoordinate(Coordinate(0, 0, 51.2497532, 4.4977063));
        auto collegeScho = std::make_shared<College>(config.generated.contactCenters++);
        collegeScho->Fill(config);
        schoten->AddContactCenter(collegeScho);

        auto kortrijk = *(geoGrid->begin() + 2);
        kortrijk->SetCoordinate(Coordinate(0, 0, 50.82900246, 3.264406009));
        auto collegeKort = std::make_shared<College>(config.generated.contactCenters++);
        collegeKort->Fill(config);
        kortrijk->AddContactCenter(collegeKort);

        geoGrid->Finalize();

        collegePopulator.Apply(geoGrid, config);

        for (const stride::Person& person : *geoGrid->GetPopulation()) {
                EXPECT_EQ(0, person.GetCollegeId());
        }
}

TEST(CollegePopulatorTest, NotCommuting)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        CollegePopulator collegePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig    config{};
        config.input.fraction_student_commutingPeople     = 0;
        config.input.fraction_1826_years_WhichAreStudents = 1;

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3, config);

        auto location = *geoGrid->begin();

        // Brasschaat and Schoten are close to each other
        // There is no commuting, but since they will still receive students from each other
        // Kortrijk will only receive students from Kortrijik
        auto brasschaat = *geoGrid->begin();
        brasschaat->SetCoordinate(Coordinate(0, 0, 51.29227, 4.49419));
        auto collegeBra = std::make_shared<College>(config.generated.contactCenters++);
        collegeBra->Fill(config);
        brasschaat->AddContactCenter(collegeBra);

        auto schoten = *(geoGrid->begin() + 1);
        schoten->SetCoordinate(Coordinate(0, 0, 51.2497532, 4.4977063));
        auto collegeScho = std::make_shared<College>(config.generated.contactCenters++);
        collegeScho->Fill(config);
        schoten->AddContactCenter(collegeScho);

        auto kortrijk = *(geoGrid->begin() + 2);
        kortrijk->SetCoordinate(Coordinate(0, 0, 50.82900246, 3.264406009));
        auto collegeKort = std::make_shared<College>(config.generated.contactCenters++);
        collegeKort->Fill(config);
        kortrijk->AddContactCenter(collegeKort);

        geoGrid->Finalize();

        collegePopulator.Apply(geoGrid, config);

        std::map<int, int> persons{
            {0, 0},     {1, 0},     {2, 0},     {3, 0},     {4, 0},     {5, 0},     {6, 0},     {7, 0},    {8, 0},
            {9, 0},     {10, 0},    {11, 0},    {12, 0},    {13, 0},    {14, 0},    {15, 0},    {16, 359}, {17, 0},
            {18, 0},    {19, 0},    {20, 0},    {21, 0},    {22, 0},    {23, 0},    {24, 0},    {25, 0},   {26, 0},
            {27, 361},  {28, 0},    {29, 0},    {30, 0},    {31, 0},    {32, 0},    {33, 0},    {34, 0},   {35, 0},
            {36, 358},  {37, 338},  {38, 0},    {39, 354},  {40, 0},    {41, 337},  {42, 0},    {43, 0},   {44, 0},
            {45, 0},    {46, 0},    {47, 0},    {48, 0},    {49, 0},    {50, 340},  {51, 0},    {52, 0},   {53, 0},
            {54, 0},    {55, 0},    {56, 0},    {57, 0},    {58, 0},    {59, 0},    {60, 0},    {61, 0},   {62, 0},
            {63, 0},    {64, 0},    {65, 349},  {66, 0},    {67, 0},    {68, 0},    {69, 0},    {70, 352}, {71, 0},
            {72, 0},    {73, 0},    {74, 0},    {75, 0},    {76, 0},    {77, 0},    {78, 0},    {79, 0},   {80, 0},
            {81, 0},    {82, 0},    {83, 0},    {84, 0},    {85, 0},    {86, 343},  {87, 0},    {88, 0},   {89, 0},
            {90, 0},    {91, 0},    {92, 0},    {93, 0},    {94, 0},    {95, 0},    {96, 0},    {97, 0},   {98, 360},
            {99, 0},    {100, 0},   {101, 0},   {102, 0},   {103, 0},   {104, 348}, {105, 0},   {106, 0},  {107, 0},
            {108, 0},   {109, 0},   {110, 0},   {111, 0},   {112, 0},   {113, 0},   {114, 0},   {115, 0},  {116, 0},
            {117, 0},   {118, 0},   {119, 0},   {120, 0},   {121, 0},   {122, 0},   {123, 0},   {124, 0},  {125, 0},
            {126, 0},   {127, 0},   {128, 0},   {129, 0},   {130, 0},   {131, 332}, {132, 0},   {133, 0},  {134, 0},
            {135, 0},   {136, 0},   {137, 0},   {138, 0},   {139, 0},   {140, 0},   {141, 0},   {142, 0},  {143, 0},
            {144, 0},   {145, 0},   {146, 0},   {147, 0},   {148, 0},   {149, 0},   {150, 0},   {151, 0},  {152, 0},
            {153, 0},   {154, 0},   {155, 0},   {156, 0},   {157, 0},   {158, 0},   {159, 0},   {160, 0},  {161, 0},
            {162, 347}, {163, 333}, {164, 0},   {165, 0},   {166, 0},   {167, 0},   {168, 0},   {169, 0},  {170, 0},
            {171, 0},   {172, 0},   {173, 0},   {174, 0},   {175, 325}, {176, 0},   {177, 0},   {178, 0},  {179, 0},
            {180, 0},   {181, 0},   {182, 0},   {183, 0},   {184, 0},   {185, 0},   {186, 0},   {187, 0},  {188, 0},
            {189, 0},   {190, 0},   {191, 0},   {192, 0},   {193, 0},   {194, 342}, {195, 0},   {196, 0},  {197, 0},
            {198, 372}, {199, 0},   {200, 368}, {201, 0},   {202, 0},   {203, 0},   {204, 0},   {205, 0},  {206, 371},
            {207, 0},   {208, 0},   {209, 0},   {210, 0},   {211, 0},   {212, 0},   {213, 0},   {214, 0},  {215, 0},
            {216, 0},   {217, 0},   {218, 370}, {219, 365}, {220, 0},   {221, 0},   {222, 0},   {223, 0},  {224, 0},
            {225, 383}, {226, 0},   {227, 0},   {228, 0},   {229, 0},   {230, 0},   {231, 0},   {232, 0},  {233, 0},
            {234, 0},   {235, 0},   {236, 369}, {237, 0},   {238, 0},   {239, 0},   {240, 0},   {241, 0},  {242, 0},
            {243, 0},   {244, 381}, {245, 0},   {246, 376}, {247, 0},   {248, 0},   {249, 0},   {250, 0},  {251, 0},
            {252, 0},   {253, 0},   {254, 0},   {255, 0},   {256, 372}, {257, 0},   {258, 0},   {259, 0},  {260, 0},
            {261, 0},   {262, 0},   {263, 0},   {264, 0},   {265, 0},   {266, 0},   {267, 0},   {268, 0},  {269, 0},
            {270, 376}, {271, 0},   {272, 372}, {273, 0},   {274, 0},   {275, 0},   {276, 379}, {277, 0},  {278, 0},
            {279, 0},   {280, 0},   {281, 0},   {282, 0},   {283, 0},   {284, 0},   {285, 382}, {286, 0},  {287, 0},
            {288, 374}, {289, 0},   {290, 373}, {291, 0},   {292, 0},   {293, 0},   {294, 0},   {295, 0},  {296, 0}};

        for (const stride::Person& person : *geoGrid->GetPopulation()) {
                EXPECT_EQ(persons[person.GetId()], person.GetCollegeId());
                if (person.IsCollegeStudentCandidate()) {
                        EXPECT_NE(0, person.GetCollegeId());
                } else {
                        EXPECT_EQ(0, person.GetCollegeId());
                }
        }

        // Assert that persons of Schoten only go to Schoten or Brasschaat
        for (const auto& household : schoten->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 325 && person->GetCollegeId() <= 364);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }

        // Assert that persons of Schoten only go to Schoten or Brasschaat
        for (const auto& household : brasschaat->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 325 && person->GetCollegeId() <= 364);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Kortijk
        for (const auto& household : kortrijk->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 365 && person->GetCollegeId() <= 384);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }
}

TEST(CollegePopulatorTest, OnlyCommuting)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        CollegePopulator collegePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig    config{};
        config.input.fraction_student_commutingPeople     = 1;
        config.input.fraction_1826_years_WhichAreStudents = 1;

        auto geoGrid = CreateGeoGrid(2, 100, 3, 50, 3, config);

        auto location = *geoGrid->begin();

        // only commuting
        auto schoten = *(geoGrid->begin());
        schoten->SetCoordinate(Coordinate(0, 0, 51.2497532, 4.4977063));
        auto collegeScho = std::make_shared<College>(config.generated.contactCenters++);
        collegeScho->Fill(config);
        schoten->AddContactCenter(collegeScho);

        auto kortrijk = *(geoGrid->begin() + 1);
        kortrijk->SetCoordinate(Coordinate(0, 0, 50.82900246, 3.264406009));
        auto collegeKort = std::make_shared<College>(config.generated.contactCenters++);
        collegeKort->Fill(config);
        kortrijk->AddContactCenter(collegeKort);

        schoten->AddOutgoingCommutingLocation(kortrijk, 0.5);
        kortrijk->AddIncomingCommutingLocation(schoten, 0.5);
        kortrijk->AddOutgoingCommutingLocation(schoten, 0.5);
        schoten->AddIncomingCommutingLocation(kortrijk, 0.5);

        geoGrid->Finalize();

        collegePopulator.Apply(geoGrid, config);

        // Assert that persons of Schoten only go to Kortrijk
        for (const auto& household : schoten->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 271 && person->GetCollegeId() <= 290);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Schoten
        for (const auto& household : kortrijk->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 251 && person->GetCollegeId() <= 270);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }
}

TEST(CollegePopulatorTest, OnlyCommutingButNoCommutingAvaiable)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        CollegePopulator collegePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig    config{};
        config.input.fraction_student_commutingPeople     = 1;
        config.input.fraction_1826_years_WhichAreStudents = 1;

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3, config);

        auto brasschaat = *geoGrid->begin();
        brasschaat->SetCoordinate(Coordinate(0, 0, 51.29227, 4.49419));
        auto collegeBra = std::make_shared<College>(config.generated.contactCenters++);
        collegeBra->Fill(config);
        brasschaat->AddContactCenter(collegeBra);

        auto schoten = *(geoGrid->begin() + 1);
        schoten->SetCoordinate(Coordinate(0, 0, 51.2497532, 4.4977063));
        auto collegeScho = std::make_shared<College>(config.generated.contactCenters++);
        collegeScho->Fill(config);
        schoten->AddContactCenter(collegeScho);

        auto kortrijk = *(geoGrid->begin() + 2);
        kortrijk->SetCoordinate(Coordinate(0, 0, 50.82900246, 3.264406009));
        auto collegeKort = std::make_shared<College>(config.generated.contactCenters++);
        collegeKort->Fill(config);
        kortrijk->AddContactCenter(collegeKort);

        // test case is only commuting but between nobody is commuting from or to Brasschaat
        schoten->AddOutgoingCommutingLocation(kortrijk, 0.5);
        kortrijk->AddIncomingCommutingLocation(schoten, 0.5);

        kortrijk->AddOutgoingCommutingLocation(schoten, 0.5);
        schoten->AddIncomingCommutingLocation(kortrijk, 0.5);

        geoGrid->Finalize();

        collegePopulator.Apply(geoGrid, config);

        // Assert that persons of Schoten only go to Kortrijk
        for (const auto& household : schoten->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 365 && person->GetCollegeId() <= 384);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }

        // Assert that persons of Brasschaat only go to Brasschaat or Schoten
        for (const auto& household : brasschaat->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 325 && person->GetCollegeId() <= 364);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Schoten
        for (const auto& household : kortrijk->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetCollegeId() >= 345 && person->GetCollegeId() <= 364);
                        } else {
                                EXPECT_EQ(0, person->GetCollegeId());
                        }
                }
        }
}

} // namespace
