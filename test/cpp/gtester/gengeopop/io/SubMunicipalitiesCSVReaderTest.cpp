#include <gengeopop/io/CitiesCSVReader.h>
#include <gengeopop/io/SubMunicipalitiesCSVReader.h>
#include <gtest/gtest.h>

using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> getExpectedGeoGrid()
{
        std::shared_ptr<GeoGrid> geoGrid = std::make_shared<GeoGrid>();
        auto                     loc1    = std::make_shared<Location>(11057, 1, 9042, Coordinate(), "MALLE");
        auto                     loc2    = std::make_shared<Location>(11009, 1, 16227, Coordinate(), "BRECHT");
        auto                     loc3    = std::make_shared<Location>(46003, 4, 28654, Coordinate(), "BEVEREN");

        loc1->calculatePopulation(10000);
        loc2->calculatePopulation(10000);
        loc3->calculatePopulation(10000);

        auto loc4 = std::make_shared<Location>(110571, 1, 0.5, Coordinate(4.733333333333333, 51.3), "OOSTMALLE");
        loc1->addSubMunicipality(loc4);
        loc4->setParent(loc1);
        auto loc5 = std::make_shared<Location>(110572, 1, Coordinate(4.683333333333334, 51.3), "WESTMALLE");
        loc1->addSubMunicipality(loc5);
        loc5->setParent(loc1);

        auto loc6 = std::make_shared<Location>(110091, 1, Coordinate(4.683333333333334, 51.35), "SINT-LENAARTS");
        loc2->addSubMunicipality(loc6);
        loc6->setParent(loc2);
        auto loc7 = std::make_shared<Location>(110092, 1, Coordinate(4.583333333333333, 51.3), "SINT-JOB-IN-'T-GOOR");
        loc2->addSubMunicipality(loc7);
        loc7->setParent(loc2);

        auto loc8 = std::make_shared<Location>(460031, 4, Coordinate(4.183333333333334, 51.25), "VERREBROEK");
        loc3->addSubMunicipality(loc8);
        loc8->setParent(loc3);
        auto loc9 =
            std::make_shared<Location>(460032, 4, Coordinate(4.166666666666667, 51.28333333333333), "KIELDRECHT");
        loc3->addSubMunicipality(loc9);
        loc9->setParent(loc3);
        auto loc10 = std::make_shared<Location>(460033, 4, Coordinate(4.266666666666667, 51.31666666666667), "DOEL");
        loc3->addSubMunicipality(loc10);
        loc10->setParent(loc3);
        auto loc11 = std::make_shared<Location>(460034, 4, Coordinate(4.283333333333333, 51.25), "KALLO");
        loc3->addSubMunicipality(loc11);
        loc11->setParent(loc3);
        auto loc12 = std::make_shared<Location>(460035, 4, Coordinate(4.2, 51.21666666666667), "VRASENE");
        loc3->addSubMunicipality(loc12);
        loc12->setParent(loc3);
        auto loc13 = std::make_shared<Location>(460036, 4, Coordinate(4.25, 51.18333333333333), "HAASDONK");
        loc3->addSubMunicipality(loc13);
        loc13->setParent(loc3);
        auto loc14 = std::make_shared<Location>(460037, 4, Coordinate(4.283333333333333, 51.21666666666667), "MELSELE");
        loc3->addSubMunicipality(loc14);
        loc14->setParent(loc3);
        auto loc15 = std::make_shared<Location>(460038, 4, Coordinate(4.25, 51.21666666666667), "BEVEREN");
        loc3->addSubMunicipality(loc15);
        loc15->setParent(loc3);

        loc4->setRelativePopulation(0.5);
        loc5->setRelativePopulation(0.5);
        loc6->setRelativePopulation(0.2687211552776537);
        loc7->setRelativePopulation(0.39794551138901296);
        loc7->setRelativePopulation(0.06391015871835855);
        loc8->setRelativePopulation(0.12417760101103967);
        loc9->setRelativePopulation(0.004618444039698175);
        loc10->setRelativePopulation(0.07379753187376872);
        loc11->setRelativePopulation(0.12528342564026318);
        loc12->setRelativePopulation(0.13923633052075976);
        loc13->setRelativePopulation(0.343976508196112);
        loc15->setRelativePopulation(0.125);

        geoGrid->addLocation(loc1);
        geoGrid->addLocation(loc2);
        geoGrid->addLocation(loc3);
        geoGrid->addLocation(loc4);
        geoGrid->addLocation(loc5);
        geoGrid->addLocation(loc6);
        geoGrid->addLocation(loc7);
        geoGrid->addLocation(loc8);
        geoGrid->addLocation(loc9);
        geoGrid->addLocation(loc10);
        geoGrid->addLocation(loc11);
        geoGrid->addLocation(loc12);
        geoGrid->addLocation(loc13);
        geoGrid->addLocation(loc14);
        geoGrid->addLocation(loc15);

        return geoGrid;
}

TEST(SubMunicipalitiesCSVReaderTest, test1)
{
        std::shared_ptr<GeoGrid> geoGrid = std::make_shared<GeoGrid>();

        geoGrid->addLocation(std::make_shared<Location>(11057, 1, 9042, Coordinate(), "MALLE"));
        geoGrid->addLocation(std::make_shared<Location>(11009, 1, 16227, Coordinate(), "BRECHT"));
        geoGrid->addLocation(std::make_shared<Location>(46003, 4, 28654, Coordinate(), "BEVEREN"));

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                loc->calculatePopulation(10000);
        }

        std::string csvString = R"( parent_id,id,population_rel_to_parent,latitude,longitude,name
11057,110571,0.5,51.3,4.733333333333333,OOSTMALLE
11057,110572,0.5,51.3,4.683333333333334,WESTMALLE
11009,110091,0.2687211552776537,51.35,4.683333333333334,SINT-LENAARTS
11009,110092,0.39794551138901296,51.3,4.583333333333333,SINT-JOB-IN-'T-GOOR
46003,460031,0.06391015871835855,51.25,4.183333333333334,VERREBROEK
46003,460032,0.12417760101103967,51.28333333333333,4.166666666666667,KIELDRECHT
46003,460033,0.004618444039698175,51.31666666666667,4.266666666666667,DOEL
46003,460034,0.07379753187376872,51.25,4.283333333333333,KALLO
46003,460035,0.12528342564026318,51.21666666666667,4.2,VRASENE
46003,460036,0.13923633052075976,51.18333333333333,4.25,HAASDONK
46003,460037,0.343976508196112,51.21666666666667,4.283333333333333,MELSELE
46003,460038,0.125,51.21666666666667,4.25,BEVEREN
)";

        std::shared_ptr<GeoGrid> expectedGeoGrid = getExpectedGeoGrid();

        auto instream = std::make_unique<std::istringstream>(csvString);

        SubMunicipalitiesCSVReader reader(std::move(instream));

        reader.FillGeoGrid(geoGrid);

        EXPECT_EQ(geoGrid->size(), expectedGeoGrid->size());
        for (const auto& loc : *geoGrid) {
                EXPECT_EQ(*loc, *(expectedGeoGrid->GetById(loc->getID())));
        }
}

} // namespace
