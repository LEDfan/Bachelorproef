#include "GeoGrid.h"
#include "generators/GeoGridGenerator.h"
#include "generators/SchoolGenerator.h"
#include "io/GeoGridJSONWriter.h"
#include <iostream>
#include <memory>
#include <gengeopop/io/CommutesReader.h>
#include <gengeopop/io/CitiesReader.h>
#include <gengeopop/io/CitiesCSVReader.h>
#include <util/FileSys.h>

using namespace gengeopop;

int main()
{

        std::ifstream file(stride::util::FileSys::GetDataDir().string() + "/flanders_cities.csv");
        std::ifstream fileCommutes(stride::util::FileSys::GetDataDir().string() + "/flanders_commuting.csv");

        CitiesCSVReader citiesReader(file);
        const auto& locs = citiesReader.getLocations();

        for (const auto& loc : locs) {
                std::cout << loc.first << "\t" << loc.second->getName() << std::endl;
        }


        CommutesReader commutesReader(fileCommutes, locs);
        const auto& commuting = commutesReader.getCommutes();

        for (const auto& commute : commuting) {
                std::cout << commute.first->getName() << " to " << commute.second.first->getName() << " proportion: " << commute.second.second << std::endl;

        }

        return 0;
}
