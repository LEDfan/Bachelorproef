/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#include <gengeopop/io/CitiesCSVReader.h>
#include <gtest/gtest.h>

using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> getExpectedGeoGrid()
{
        auto                     pop     = stride::Population::Create();
        std::shared_ptr<GeoGrid> geoGrid = std::make_shared<GeoGrid>(pop.get());
        auto loc1 = std::make_shared<Location>(11002, 1, Coordinate(4.413545489, 51.2165845), "ANTWERPEN");

        auto loc2 = std::make_shared<Location>(11007, 1, Coordinate(4.4856299, 51.1966684), "BORSBEEK");

        auto loc3 = std::make_shared<Location>(11008, 1, Coordinate(4.49419, 51.29227), "BRASSCHAAT");

        auto loc4 = std::make_shared<Location>(11009, 1, Coordinate(4.6376322, 51.3495775), "BRECHT");

        auto loc5 = std::make_shared<Location>(11016, 1, Coordinate(4.47518, 51.38298), "ESSEN");

        auto loc6 = std::make_shared<Location>(73107, 7, Coordinate(5.70979219, 50.96991794), "MAASMECHELEN");

        auto loc7 =
            std::make_shared<Location>(73109, 7, Coordinate(5.806343076, 50.74921941), "VOEREN-'S GRAVENVOEREN");

        loc1->SetRelativePopulation(0.76599210042448873);
        loc2->SetRelativePopulation(0.018849454066692393);
        loc3->SetRelativePopulation(0.065934783102172378);
        loc4->SetRelativePopulation(0.04604396976369373);
        loc5->SetRelativePopulation(0.029663133044287561);
        loc6->SetRelativePopulation(0.06618731981930856);
        loc7->SetRelativePopulation(0.0073292397793566838);

        geoGrid->AddLocation(loc1);
        geoGrid->AddLocation(loc2);
        geoGrid->AddLocation(loc3);
        geoGrid->AddLocation(loc4);
        geoGrid->AddLocation(loc5);
        geoGrid->AddLocation(loc6);
        geoGrid->AddLocation(loc7);

        return geoGrid;
}

TEST(CitiesCSVReaderTest, test1)
{
        std::string csvString = R"(id,province,population,x_coord,y_coord,latitude,longitude,name
11002,1,269954,153104.586,212271.7101,51.2165845,4.413545489,ANTWERPEN
11007,1,6643,158373.2671,209240.2514,51.1966684,4.4856299,BORSBEEK
11008,1,23237,158918.8198,221748.3274,51.29227,4.49419,BRASSCHAAT
11009,1,16227,168070.1501,224780.0715,51.3495775,4.6376322,BRECHT
11016,1,10454,156460.2596,237847.8004,51.38298,4.47518,ESSEN
73107,7,23326,243578.1573,184975.9303,50.96991794,5.70979219,MAASMECHELEN
73109,7,2583,251986.4171,160699.7771,50.74921941,5.806343076,VOEREN-'S GRAVENVOEREN
)";

        std::shared_ptr<GeoGrid> expectedGeoGrid = getExpectedGeoGrid();
        auto                     pop             = stride::Population::Create();
        std::shared_ptr<GeoGrid> geoGrid         = std::make_shared<GeoGrid>(pop.get());

        auto instream = std::make_unique<std::istringstream>(csvString);

        CitiesCSVReader reader(std::move(instream));

        reader.FillGeoGrid(geoGrid);

        for (const auto& loc : *geoGrid) {
                EXPECT_EQ(*loc, *(expectedGeoGrid->GetById(loc->GetID())));
                EXPECT_DOUBLE_EQ(loc->GetRelativePopulationSize(),
                                 (expectedGeoGrid->GetById(loc->GetID()))->GetRelativePopulationSize());
        }
}

} // namespace
