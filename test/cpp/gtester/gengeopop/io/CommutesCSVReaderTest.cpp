#include <gengeopop/io/CommutesCSVReader.h>
#include <gengeopop/io/HouseholdCSVReader.h>
#include <gtest/gtest.h>
#include <memory>

using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> getExpectedGeoGrid()
{
        std::shared_ptr<GeoGrid> geoGrid = std::make_shared<GeoGrid>();
        geoGrid->addLocation(std::make_shared<Location>(21, 0, 1000));
        geoGrid->addLocation(std::make_shared<Location>(22, 0, 800));
        geoGrid->addLocation(std::make_shared<Location>(23, 0, 900));
        geoGrid->addLocation(std::make_shared<Location>(24, 0, 1300));

        // to 21
        geoGrid->GetById(21)->addIncomingCommutingLocation(geoGrid->GetById(22), 0.18218018914883027);
        geoGrid->GetById(22)->addOutgoingCommutingLocation(geoGrid->GetById(21), 0.18218018914883027);

        geoGrid->GetById(21)->addIncomingCommutingLocation(geoGrid->GetById(23), 0.33250373320059728);
        geoGrid->GetById(23)->addOutgoingCommutingLocation(geoGrid->GetById(21), 0.33250373320059728);

        geoGrid->GetById(21)->addIncomingCommutingLocation(geoGrid->GetById(24), 0.21154803384768542);
        geoGrid->GetById(24)->addOutgoingCommutingLocation(geoGrid->GetById(21), 0.21154803384768542);

        // to 22
        geoGrid->GetById(22)->addIncomingCommutingLocation(geoGrid->GetById(21), 0.087741132545115119);
        geoGrid->GetById(21)->addOutgoingCommutingLocation(geoGrid->GetById(22), 0.087741132545115119);

        geoGrid->GetById(22)->addIncomingCommutingLocation(geoGrid->GetById(24), 0.4387056627255756);
        geoGrid->GetById(24)->addOutgoingCommutingLocation(geoGrid->GetById(22), 0.4387056627255756);

        // to 23
        geoGrid->GetById(23)->addIncomingCommutingLocation(geoGrid->GetById(21), 0.29533050333535477);
        geoGrid->GetById(21)->addOutgoingCommutingLocation(geoGrid->GetById(23), 0.29533050333535477);

        geoGrid->GetById(23)->addIncomingCommutingLocation(geoGrid->GetById(22), 0.42449969678593086);
        geoGrid->GetById(22)->addOutgoingCommutingLocation(geoGrid->GetById(23), 0.42449969678593086);

        // to 24
        geoGrid->GetById(24)->addIncomingCommutingLocation(geoGrid->GetById(22), 1);
        geoGrid->GetById(22)->addOutgoingCommutingLocation(geoGrid->GetById(24), 1);

        return geoGrid;
}

TEST(CommutesCSVReaderTest, test1)
{
        std::string csvString = "id_21,id_22,id_23,id_24\n"
                                "550,366,668,425\n" // to 21
                                "141,761,0,705\n"   // to 22
                                "487,700,462,0\n"   // to 23
                                "0,611,0,0\n";      // to 24

        std::shared_ptr<GeoGrid> expectedGeoGrid = getExpectedGeoGrid();
        std::shared_ptr<GeoGrid> geoGrid         = std::make_shared<GeoGrid>();
        geoGrid->addLocation(std::make_shared<Location>(21, 0, 1000));
        geoGrid->addLocation(std::make_shared<Location>(22, 0, 800));
        geoGrid->addLocation(std::make_shared<Location>(23, 0, 900));
        geoGrid->addLocation(std::make_shared<Location>(24, 0, 1300));

        auto instream = std::make_unique<std::istringstream>(csvString);

        CommutesCSVReader reader(std::move(instream));

        reader.FillGeoGrid(geoGrid);

        for (const auto& loc : *geoGrid) {
                int         i                = 0;
                const auto& expectedLoc      = expectedGeoGrid->GetById(loc->getID());
                const auto& outGoingExpected = expectedLoc->getOutgoingCommuningCities();
                for (const auto& commute : loc->getOutgoingCommuningCities()) {
                        EXPECT_DOUBLE_EQ(outGoingExpected[i].first->getID(), commute.first->getID());
                        EXPECT_DOUBLE_EQ(outGoingExpected[i].second, commute.second);
                        i++;
                }
                i                            = 0;
                const auto& incomingExpected = expectedLoc->getIncomingCommuningCities();
                for (const auto& commute : loc->getIncomingCommuningCities()) {
                        EXPECT_DOUBLE_EQ(incomingExpected[i].first->getID(), commute.first->getID());
                        EXPECT_DOUBLE_EQ(incomingExpected[i].second, commute.second);
                        i++;
                }

                EXPECT_DOUBLE_EQ(expectedLoc->outGoingCommutingPeople(1), loc->outGoingCommutingPeople(1));
                EXPECT_DOUBLE_EQ(expectedLoc->incomingCommutingPeople(1), loc->incomingCommutingPeople(1));
        }
}

} // namespace
