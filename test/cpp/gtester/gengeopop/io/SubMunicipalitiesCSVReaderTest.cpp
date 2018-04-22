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

        loc1->CalculatePopulation(10000);
        loc2->CalculatePopulation(10000);
        loc3->CalculatePopulation(10000);

        auto loc4 = std::make_shared<Location>(110571, 1, 0.5, Coordinate(4.733333333333333, 51.3), "OOSTMALLE");
        loc1->AddSubMunicipality(loc4);
        loc4->SetParent(loc1);
        auto loc5 = std::make_shared<Location>(110572, 1, Coordinate(4.683333333333334, 51.3), "WESTMALLE");
        loc1->AddSubMunicipality(loc5);
        loc5->SetParent(loc1);

        auto loc6 = std::make_shared<Location>(110091, 1, Coordinate(4.683333333333334, 51.35), "SINT-LENAARTS");
        loc2->AddSubMunicipality(loc6);
        loc6->SetParent(loc2);
        auto loc7 = std::make_shared<Location>(110092, 1, Coordinate(4.583333333333333, 51.3), "SINT-JOB-IN-'T-GOOR");
        loc2->AddSubMunicipality(loc7);
        loc7->SetParent(loc2);

        auto loc8 = std::make_shared<Location>(460031, 4, Coordinate(4.183333333333334, 51.25), "VERREBROEK");
        loc3->AddSubMunicipality(loc8);
        loc8->SetParent(loc3);
        auto loc9 =
            std::make_shared<Location>(460032, 4, Coordinate(4.166666666666667, 51.28333333333333), "KIELDRECHT");
        loc3->AddSubMunicipality(loc9);
        loc9->SetParent(loc3);
        auto loc10 = std::make_shared<Location>(460033, 4, Coordinate(4.266666666666667, 51.31666666666667), "DOEL");
        loc3->AddSubMunicipality(loc10);
        loc10->SetParent(loc3);
        auto loc11 = std::make_shared<Location>(460034, 4, Coordinate(4.283333333333333, 51.25), "KALLO");
        loc3->AddSubMunicipality(loc11);
        loc11->SetParent(loc3);
        auto loc12 = std::make_shared<Location>(460035, 4, Coordinate(4.2, 51.21666666666667), "VRASENE");
        loc3->AddSubMunicipality(loc12);
        loc12->SetParent(loc3);
        auto loc13 = std::make_shared<Location>(460036, 4, Coordinate(4.25, 51.18333333333333), "HAASDONK");
        loc3->AddSubMunicipality(loc13);
        loc13->SetParent(loc3);
        auto loc14 = std::make_shared<Location>(460037, 4, Coordinate(4.283333333333333, 51.21666666666667), "MELSELE");
        loc3->AddSubMunicipality(loc14);
        loc14->SetParent(loc3);
        auto loc15 = std::make_shared<Location>(460038, 4, Coordinate(4.25, 51.21666666666667), "BEVEREN");
        loc3->AddSubMunicipality(loc15);
        loc15->SetParent(loc3);

        loc4->SetRelativePopulation(0.5);
        loc5->SetRelativePopulation(0.5);
        loc6->SetRelativePopulation(0.2687211552776537);
        loc7->SetRelativePopulation(0.39794551138901296);
        loc7->SetRelativePopulation(0.06391015871835855);
        loc8->SetRelativePopulation(0.12417760101103967);
        loc9->SetRelativePopulation(0.004618444039698175);
        loc10->SetRelativePopulation(0.07379753187376872);
        loc11->SetRelativePopulation(0.12528342564026318);
        loc12->SetRelativePopulation(0.13923633052075976);
        loc13->SetRelativePopulation(0.343976508196112);
        loc15->SetRelativePopulation(0.125);

        geoGrid->AddLocation(loc1);
        geoGrid->AddLocation(loc2);
        geoGrid->AddLocation(loc3);
        geoGrid->AddLocation(loc4);
        geoGrid->AddLocation(loc5);
        geoGrid->AddLocation(loc6);
        geoGrid->AddLocation(loc7);
        geoGrid->AddLocation(loc8);
        geoGrid->AddLocation(loc9);
        geoGrid->AddLocation(loc10);
        geoGrid->AddLocation(loc11);
        geoGrid->AddLocation(loc12);
        geoGrid->AddLocation(loc13);
        geoGrid->AddLocation(loc14);
        geoGrid->AddLocation(loc15);

        return geoGrid;
}

TEST(SubMunicipalitiesCSVReaderTest, test1)
{
        std::shared_ptr<GeoGrid> geoGrid = std::make_shared<GeoGrid>();

        geoGrid->AddLocation(std::make_shared<Location>(11057, 1, 9042, Coordinate(), "MALLE"));
        geoGrid->AddLocation(std::make_shared<Location>(11009, 1, 16227, Coordinate(), "BRECHT"));
        geoGrid->AddLocation(std::make_shared<Location>(46003, 4, 28654, Coordinate(), "BEVEREN"));

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                loc->CalculatePopulation(10000);
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
                EXPECT_EQ(*loc, *(expectedGeoGrid->GetById(loc->GetID())));
        }
}

} // namespace
