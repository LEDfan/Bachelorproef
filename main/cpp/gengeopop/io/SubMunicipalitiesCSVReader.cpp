#include "SubMunicipalitiesCSVReader.h"
#include "../../util/CSV.h"
#include "../../util/CSVRow.h"
#include <iostream>

namespace gengeopop {

SubMunicipalitiesCSVReader::SubMunicipalitiesCSVReader(std::unique_ptr<std::istream> inputStream) : CitiesReader(std::move(inputStream)) {}

void SubMunicipalitiesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{

        // columns: parent_nis,nis,parent_is_sub,population_rel_to_parent,latitude,longitude,name
        stride::util::CSV reader(*(m_inputStream.get()));

        // first build a list of relative population sizes of each Location
        std::map<std::shared_ptr<Location>, double> populationSizes;

        for (const auto& loc : *geoGrid) {
                populationSizes[loc] = loc->getRe
        }



//        for (const stride::util::CSVRow& row : reader) {
//                auto id       = row.getValue<int>(0);
//                auto location = std::make_shared<Location>(id,                                 // id
//                                                           row.getValue<int>(1),               // province
//                                                           row.getValue<int>(2),               // relative population
//                                                           Coordinate(row.getValue<double>(3), // x_coord
//                                                                      row.getValue<double>(4), // y_coord
//                                                                      row.getValue<double>(6), // longtitude
//                                                                      row.getValue<double>(5)  // latitude
//                                                                      ),
//                                                           row.getValue(7));
//                geoGrid->addLocation(location);
//        }
}
} // namespace gengeopop
