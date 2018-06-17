#include "createGeogrid.h"
#include <gengeopop/College.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/K12School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/populators/CollegePopulator.h>
#include <gengeopop/populators/WorkplacePopulator.h>
#include <gtest/gtest.h>
#include <util/LogUtils.h>
#include <util/RNManager.h>

using namespace gengeopop;
using namespace stride;

namespace {

TEST(WorkplacePopulatorTest, NoPopulation)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        auto geoGrid = std::make_shared<GeoGrid>();

        geoGrid->AddLocation(std::make_shared<Location>(0, 0, 0));

        WorkplacePopulator workplacePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));

        GeoGridConfig config{};

        geoGrid->Finalize();

        EXPECT_NO_THROW(workplacePopulator.Apply(geoGrid, config));
}

TEST(WorkplacePopulatorTest, NoActive)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplacePopulator workplacePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig      config{};
        config.input.fraction_1865_years_active           = 0;
        config.input.fraction_1826_years_WhichAreStudents = 1;

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3);

        auto location = *geoGrid->begin();

        // Brasschaat and Schoten are close to each other
        // There is no commuting, but since they will still receive students from each other
        // Kortrijk will only receive students from Kortrijik
        auto brasschaat = *geoGrid->begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));

        auto schoten = *(geoGrid->begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));

        auto kortrijk = *(geoGrid->begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));

        geoGrid->Finalize();

        workplacePopulator.Apply(geoGrid, config);

        for (const stride::Person& person : *geoGrid->GetPopulation()) {
                EXPECT_EQ(0, person.GetWorkId());
        }
}

TEST(WorkplacePopulatorTest, NoCommuting)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplacePopulator workplacePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig      config{};
        config.input.fraction_active_commutingPeople      = 0;
        config.input.fraction_1865_years_active           = 1;
        config.input.fraction_1826_years_WhichAreStudents = 0.5;

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3);

        // Brasschaat and Schoten are close to each other
        // There is no commuting, but since they will still receive students from each other
        // Kortrijk will only receive students from Kortrijik
        auto brasschaat = *geoGrid->begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));

        auto workBra1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workBra1->Fill(geoGrid);
        brasschaat->AddContactCenter(workBra1);

        auto workBra2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workBra2->Fill(geoGrid);
        brasschaat->AddContactCenter(workBra2);

        auto schoten = *(geoGrid->begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));

        auto workScho1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workScho1->Fill(geoGrid);
        schoten->AddContactCenter(workScho1);

        auto workScho2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workScho2->Fill(geoGrid);
        schoten->AddContactCenter(workScho2);

        auto kortrijk = *(geoGrid->begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));

        auto workKor1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workKor1->Fill(geoGrid);
        kortrijk->AddContactCenter(workKor1);

        auto workKor2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workKor2->Fill(geoGrid);
        kortrijk->AddContactCenter(workKor2);

        geoGrid->Finalize();

        workplacePopulator.Apply(geoGrid, config);

        // Assert that persons of Schoten only go to Schoten or Brasschaat
        for (const auto& household : schoten->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 325 && person->GetWorkId() <= 328);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 325 && person->GetWorkId() <= 328) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }

        // Assert that persons of Schoten only go to Schoten or Brasschaat
        for (const auto& household : brasschaat->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 325 && person->GetWorkId() <= 328);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 325 && person->GetWorkId() <= 328) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Kortijk
        for (const auto& household : kortrijk->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 329 && person->GetWorkId() <= 330);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 329 && person->GetWorkId() <= 330) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }
}

TEST(WorkplacePopulatorTest, OnlyCommuting)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplacePopulator workplacePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig      config{};
        config.input.fraction_active_commutingPeople      = 0;
        config.input.fraction_active_commutingPeople      = 1;
        config.input.fraction_student_commutingPeople     = 0;
        config.calculated._1865_and_years_active          = 1;
        config.input.fraction_1865_years_active           = 1;
        config.input.fraction_1826_years_WhichAreStudents = 0.5;

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3);

        // only commuting
        auto schoten = *(geoGrid->begin());
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));

        auto workScho1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workScho1->Fill(geoGrid);
        schoten->AddContactCenter(workScho1);

        auto workScho2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workScho2->Fill(geoGrid);
        schoten->AddContactCenter(workScho2);

        auto kortrijk = *(geoGrid->begin() + 1);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));

        auto workKor1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workKor1->Fill(geoGrid);
        kortrijk->AddContactCenter(workKor1);

        auto workKor2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workKor2->Fill(geoGrid);
        kortrijk->AddContactCenter(workKor2);

        schoten->AddOutgoingCommutingLocation(kortrijk, 0.5);
        kortrijk->AddIncomingCommutingLocation(schoten, 0.5);
        kortrijk->AddOutgoingCommutingLocation(schoten, 0.5);
        schoten->AddIncomingCommutingLocation(kortrijk, 0.5);

        geoGrid->Finalize();

        workplacePopulator.Apply(geoGrid, config);

        // Assert that persons of Schoten only go to Kortrijk
        for (const auto& household : schoten->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 327 && person->GetWorkId() <= 328);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 327 && person->GetWorkId() <= 328) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Schoten
        for (const auto& household : kortrijk->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 325 && person->GetWorkId() <= 326);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 325 && person->GetWorkId() <= 326) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }
}

TEST(WorkplacePopulatorTest, OnlyCommutingButNoCommutingAvaiable)
{
        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 2;
        stride::util::RNManager rnManager(rnInfo);

        WorkplacePopulator workplacePopulator(rnManager, stride::util::LogUtils::CreateNullLogger("nullLogger"));
        GeoGridConfig      config{};
        config.input.fraction_active_commutingPeople      = 0;
        config.input.fraction_active_commutingPeople      = 1;
        config.input.fraction_student_commutingPeople     = 0;
        config.calculated._1865_and_years_active          = 1;
        config.input.fraction_1865_years_active           = 1;
        config.input.fraction_1826_years_WhichAreStudents = 0.5;

        auto geoGrid = CreateGeoGrid(3, 100, 3, 33, 3);

        auto brasschaat = *geoGrid->begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));

        auto workBra1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workBra1->Fill(geoGrid);
        brasschaat->AddContactCenter(workBra1);

        auto workBra2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workBra2->Fill(geoGrid);
        brasschaat->AddContactCenter(workBra2);

        auto schoten = *(geoGrid->begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));

        auto workScho1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workScho1->Fill(geoGrid);
        schoten->AddContactCenter(workScho1);

        auto workScho2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workScho2->Fill(geoGrid);
        schoten->AddContactCenter(workScho2);

        auto kortrijk = *(geoGrid->begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));

        auto workKor1 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workKor1->Fill(geoGrid);
        kortrijk->AddContactCenter(workKor1);

        auto workKor2 = std::make_shared<Workplace>(config.generated.contactCenters++);
        workKor2->Fill(geoGrid);
        kortrijk->AddContactCenter(workKor2);

        // test case is only commuting but between nobody is commuting from or to Brasschaat
        schoten->AddOutgoingCommutingLocation(kortrijk, 0.5);
        kortrijk->AddIncomingCommutingLocation(schoten, 0.5);

        kortrijk->AddOutgoingCommutingLocation(schoten, 0.5);
        schoten->AddIncomingCommutingLocation(kortrijk, 0.5);

        geoGrid->Finalize();

        workplacePopulator.Apply(geoGrid, config);

        // Assert that persons of Schoten only go to Kortrijk
        for (const auto& household : schoten->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 329 && person->GetWorkId() <= 330);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 329 && person->GetWorkId() <= 330) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }

        // Assert that persons of Brasschaat only go to Brasschaat or Schoten
        for (const auto& household : brasschaat->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 325 && person->GetWorkId() <= 328);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 325 && person->GetWorkId() <= 328) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Schoten
        for (const auto& household : kortrijk->GetContactCentersOfType<Household>()) {
                for (auto person : *household->GetPools()[0]) {
                        if (person->IsWorkableCandidate() && !person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE(person->GetWorkId() >= 327 && person->GetWorkId() <= 328);
                        } else if (person->IsCollegeStudentCandidate()) {
                                EXPECT_TRUE((person->GetWorkId() >= 327 && person->GetWorkId() <= 328) ||
                                            person->GetWorkId() == 0);
                        } else {
                                EXPECT_EQ(0, person->GetWorkId());
                        }
                }
        }
}

} // namespace
