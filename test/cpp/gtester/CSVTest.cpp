#include <gtest/gtest.h>
#include <util/CSV.h>

using namespace stride::util;

TEST(CSVTest, SafeCastTest) {
        EXPECT_THROW(safe_cast<int>("10test"), boost::bad_lexical_cast);
        EXPECT_THROW(safe_cast<int>("abc"), boost::bad_lexical_cast);
        EXPECT_THROW(safe_cast<unsigned int>("-100"), boost::bad_numeric_cast);
        EXPECT_THROW(safe_cast<uint8_t>("1024"), boost::bad_numeric_cast);
        EXPECT_THROW(safe_cast<double>("10.10.1"), boost::bad_lexical_cast);

        EXPECT_EQ(safe_cast<int>("10"), 10);
        EXPECT_EQ(safe_cast<unsigned int>("100"), 100);
        EXPECT_EQ(safe_cast<uint8_t>("125"), 125);
        EXPECT_EQ(safe_cast<double>("100.10"), 100.10);
        EXPECT_EQ(safe_cast<std::string>(std::string("abc")), "abc");
}


TEST(CSVTest, TestBadCast) {
        std::string csvString = "id,naam,col3,col4,col5\n"
                "10test,abc,-100,1024,100.10.1\n"
                "10,abc,100,125,100.10";

        std::istringstream instream(csvString);

        CSV reader(instream);

        const auto& row1 = reader.begin();
        EXPECT_THROW(row1->getValue<int>("id"), boost::bad_lexical_cast);
        EXPECT_THROW(row1->getValue<int>("naam"), boost::bad_lexical_cast);
        EXPECT_THROW(row1->getValue<unsigned int>("col3"), boost::bad_numeric_cast);
        EXPECT_THROW(row1->getValue<uint8_t>("col4"), boost::bad_numeric_cast);
        EXPECT_THROW(row1->getValue<double>("col5"), boost::bad_lexical_cast);

        const auto& row2 = reader.begin() + 1;
        EXPECT_EQ(row2->getValue<int>("id"), 10);
        EXPECT_EQ(row2->getValue("naam"), "abc");
        EXPECT_EQ(row2->getValue<unsigned int>("col3"), 100);
        EXPECT_EQ(row2->getValue<uint8_t>("col4"), 125);
        EXPECT_EQ(row2->getValue<double>("col5"), 100.10);

}
