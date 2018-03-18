#include <gengeopop/io/CitiesCSVReader.h>
#include <gengeopop/io/CommutesCSVReader.h>
#include <gengeopop/io/HouseholdCSVReader.h>
#include <gengeopop/io/ReaderFactory.h>
#include <gtest/gtest.h>
#include <memory>

using namespace gengeopop;

namespace {

TEST(ReaderFactoryTest, TestCommutes)
{
        ReaderFactory readerFactory;

        const std::shared_ptr<CommutesReader>& res1 =
            readerFactory.CreateCommutesReader(std::string("flanders_cities.csv"));

        EXPECT_NE(std::dynamic_pointer_cast<CommutesCSVReader>(res1), nullptr);

        EXPECT_THROW(readerFactory.CreateCommutesReader(std::string("test.txt")), std::runtime_error);
}

TEST(ReaderFactoryTest, TestCities)
{
        ReaderFactory readerFactory;

        const std::shared_ptr<CitiesReader>& res1 =
            readerFactory.CreateCitiesReader(std::string("flanders_cities.csv"));

        EXPECT_NE(std::dynamic_pointer_cast<CitiesCSVReader>(res1), nullptr);

        EXPECT_THROW(readerFactory.CreateCitiesReader(std::string("test.txt")), std::runtime_error);
}

TEST(ReaderFactoryTest, TestHouseHolds)
{
        ReaderFactory readerFactory;

        const std::shared_ptr<HouseholdReader>& res1 =
            readerFactory.CreateHouseholdReader(std::string("flanders_cities.csv"));

        EXPECT_NE(std::dynamic_pointer_cast<HouseholdCSVReader>(res1), nullptr);

        EXPECT_THROW(readerFactory.CreateHouseholdReader(std::string("test.txt")), std::runtime_error);
}
} // namespace
