#include <gengeopop/io/HouseholdCSVReader.h>
#include <gtest/gtest.h>
#include <memory>
#include <gengeopop/io/CommutesCSVReader.h>

using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> getExpectedGeoGrid() {
        std::shared_ptr<GeoGrid> geoGrid = std::make_shared<GeoGrid>();
        geoGrid->addLocation(std::make_shared<Location>(21, 0, 1000));
        geoGrid->addLocation(std::make_shared<Location>(22, 0, 800));
        geoGrid->addLocation(std::make_shared<Location>(23, 0, 900));
        geoGrid->addLocation(std::make_shared<Location>(24, 0, 1300));

        // to 21
        geoGrid->getById(21)->addIncomingCommutingLocation(geoGrid->getById(22), (double) 366 / (double) 800);
        geoGrid->getById(22)->addOutgoingCommutingLocation(geoGrid->getById(21), (double) 366 / (double) 800);

        geoGrid->getById(21)->addIncomingCommutingLocation(geoGrid->getById(23), (double) 668 / (double) 900);
        geoGrid->getById(23)->addOutgoingCommutingLocation(geoGrid->getById(21), (double) 668 / (double) 900);

        geoGrid->getById(21)->addIncomingCommutingLocation(geoGrid->getById(24), (double) 425 / (double) 1300);
        geoGrid->getById(24)->addOutgoingCommutingLocation(geoGrid->getById(21), (double) 425 / (double) 1300);


        // to 22
        geoGrid->getById(22)->addIncomingCommutingLocation(geoGrid->getById(21), (double) 141 / (double) 1000);
        geoGrid->getById(21)->addOutgoingCommutingLocation(geoGrid->getById(22), (double) 141 / (double) 1000);

        geoGrid->getById(22)->addIncomingCommutingLocation(geoGrid->getById(24), (double) 705 / (double) 1300);
        geoGrid->getById(24)->addOutgoingCommutingLocation(geoGrid->getById(22), (double) 705 / (double) 1300);

        // to 23
        geoGrid->getById(23)->addIncomingCommutingLocation(geoGrid->getById(21), (double) 487 / (double) 1000);
        geoGrid->getById(21)->addOutgoingCommutingLocation(geoGrid->getById(23), (double) 487 / (double) 1000);

        geoGrid->getById(23)->addIncomingCommutingLocation(geoGrid->getById(22), (double) 700 / (double) 800);
        geoGrid->getById(22)->addOutgoingCommutingLocation(geoGrid->getById(23), (double) 700 / (double) 800);

        // to 24
        geoGrid->getById(24)->addIncomingCommutingLocation(geoGrid->getById(22), (double) 611 / (double) 800);
        geoGrid->getById(22)->addOutgoingCommutingLocation(geoGrid->getById(24), (double) 611 / (double) 800);

        return geoGrid;
}

TEST(CommutesCSVReaderTest, test1)
{
        std::string csvString = "id_21,id_22,id_23,id_24\n"
                "550,366,668,425\n"  // to 21
                "141,761,0,705\n"    // to 22
                "487,700,462,0\n"  // to 23
                "0,611,0,0\n"; // to 24

        std::shared_ptr<GeoGrid> expectedGeoGrid = getExpectedGeoGrid();
        std::shared_ptr<GeoGrid> geoGrid = std::make_shared<GeoGrid>();
        geoGrid->addLocation(std::make_shared<Location>(21, 0, 1000));
        geoGrid->addLocation(std::make_shared<Location>(22, 0, 800));
        geoGrid->addLocation(std::make_shared<Location>(23, 0, 900));
        geoGrid->addLocation(std::make_shared<Location>(24, 0, 1300));

        std::istringstream instream(csvString);

        CommutesCSVReader reader(instream);

        reader.fillGeoGrid(geoGrid);

        for (const auto& loc : *geoGrid) {
                int i = 0;
                const auto& expectedLoc = expectedGeoGrid->getById(loc->getID());
                const auto& outGoingExpected = expectedLoc->getOutgoingCommuningCities();
                for (const auto& commute : loc->getOutgoingCommuningCities()) {
                        EXPECT_EQ(outGoingExpected[i].first->getID(), commute.first->getID());
                        EXPECT_EQ(outGoingExpected[i].second, commute.second);
                        i++;
                }
                i = 0;
                const auto& incomingExpected = expectedLoc->getIncomingCommuningCities();
                for (const auto& commute : loc->getIncomingCommuningCities()) {
                        EXPECT_EQ(incomingExpected[i].first->getID(), commute.first->getID());
                        EXPECT_EQ(incomingExpected[i].second, commute.second);
                        i++;
                }

                EXPECT_EQ(expectedLoc->outGoingCommutingPeople(1), loc->outGoingCommutingPeople(1));
                EXPECT_EQ(expectedLoc->incomingCommutingPeople(1), loc->incomingCommutingPeople(1));
        }

}

} // namespace
