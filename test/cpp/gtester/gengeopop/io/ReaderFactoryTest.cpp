#include <gengeopop/io/CitiesCSVReader.h>
#include <gengeopop/io/CommutesCSVReader.h>
#include <gengeopop/io/HouseholdCSVReader.h>
#include <gengeopop/io/ReaderFactory.h>
#include <gtest/gtest.h>
#include <memory>
#include <util/FileSys.h>

using namespace gengeopop;

namespace {

TEST(ReaderFactoryTest, TestCommutes)
{
        ReaderFactory readerFactory;

        const std::shared_ptr<CommutesReader>& res1 =
            readerFactory.CreateCommutesReader(std::string("flanders_cities.csv"));

        EXPECT_NE(std::dynamic_pointer_cast<CommutesCSVReader>(res1), nullptr);
        EXPECT_THROW(
            readerFactory.CreateCommutesReader(stride::util::FileSys::GetTestsDir() / "/testdata/io/empty.txt"),
            std::runtime_error);
}

TEST(ReaderFactoryTest, TestCommutesFromFile)
{
        ReaderFactory readerFactory;

        const std::shared_ptr<CommutesReader>& res2 =
            readerFactory.CreateCommutesReader(stride::util::FileSys::GetTestsDir() / "/testdata/io/commutes.csv");

        auto geoGrid = std::make_shared<GeoGrid>();
        geoGrid->addLocation(std::make_shared<Location>(21, 0, 1000));
        geoGrid->addLocation(std::make_shared<Location>(22, 0, 1000));

        res2->FillGeoGrid(geoGrid);

        EXPECT_EQ(geoGrid->GetById(21)->incomingCommutingPeople(1.0), 200);
        EXPECT_EQ(geoGrid->GetById(22)->outGoingCommutingPeople(1.0), 200);
        EXPECT_EQ(geoGrid->GetById(21)->incomingCommutingPeople(1.0), 200);
        EXPECT_EQ(geoGrid->GetById(22)->outGoingCommutingPeople(1.0), 200);
}

TEST(ReaderFactoryTest, TestCities)
{
        ReaderFactory readerFactory;

        const std::shared_ptr<CitiesReader>& res1 =
            readerFactory.CreateCitiesReader(std::string("flanders_cities.csv"));

        EXPECT_NE(std::dynamic_pointer_cast<CitiesCSVReader>(res1), nullptr);

        EXPECT_THROW(readerFactory.CreateCitiesReader(stride::util::FileSys::GetTestsDir() / "/testdata/io/empty.txt"),
                     std::runtime_error);
        EXPECT_THROW(readerFactory.CreateCitiesReader(stride::util::FileSys::GetTestsDir() / "/testdata/io/random.txt"),
                     std::invalid_argument);
}

TEST(ReaderFactoryTest, TestHouseHolds)
{
        ReaderFactory readerFactory;

        const std::shared_ptr<HouseholdReader>& res1 =
            readerFactory.CreateHouseholdReader(std::string("flanders_cities.csv"));

        EXPECT_NE(std::dynamic_pointer_cast<HouseholdCSVReader>(res1), nullptr);

        EXPECT_THROW(
            readerFactory.CreateHouseholdReader(stride::util::FileSys::GetTestsDir() / "/testdata/io/empty.txt"),
            std::runtime_error);
}
} // namespace
