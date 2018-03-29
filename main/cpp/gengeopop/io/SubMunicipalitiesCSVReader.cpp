#include "SubMunicipalitiesCSVReader.h"
#include "../../util/CSV.h"
#include "../../util/CSVRow.h"
#include <iostream>

namespace gengeopop {

SubMunicipalitiesCSVReader::SubMunicipalitiesCSVReader(std::unique_ptr<std::istream> inputStream) : SubMunicipalitiesReader(std::move(inputStream)) {}

void SubMunicipalitiesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{

        // columns: parent_nis,nis,parent_is_sub,population_rel_to_parent,latitude,longitude,name
        stride::util::CSV reader(*(m_inputStream.get()));

        // first build a list of relative population sizes of each Location
        std::map<unsigned int, double> populationSizesMapping;
        std::map<unsigned int, int> provinceMapping;

        for (const auto& loc : *geoGrid) {
                populationSizesMapping[loc->getID()] = loc->getRelativePopulationSize();
                provinceMapping[loc->getID()] = loc->getProvince();
        }

        for (const stride::util::CSVRow& row : reader) {
                auto parentId       = row.getValue<unsigned int>(0);
                auto id = row.getValue<unsigned int>(1);

                if (id == parentId && row.getValue<std::string>("parent_is_sub") == "True") {
                        // the parent is a city -> and the current row is that city
                        // we will adapt the currently "parent" city to become a sub municipality
                        auto parent = geoGrid->GetById(id);
                        parent->setRelativePopulation(row.getValue<double>("population_rel_to_parent") * parent->getRelativePopulationSize());
                        continue;
                }

                if (row.getValue<std::string>("parent_is_sub") == "False") {
                        // the parent is not a city -> remove it
                        try {
                                auto parent = geoGrid->GetById(parentId);
                                geoGrid->remove(parent);
                        } catch (const std::out_of_range& ex) {
                                // already deleted -> ignore
                        }
                }

                auto location = std::make_shared<Location>(id,                                 // id
                                                           provinceMapping[parentId],               // province
                                                           Coordinate(0, // x_coord
                                                                      0, // y_coord
                                                                      row.getValue<double>("longitude"), // longtitude
                                                                      row.getValue<double>("latitude")  // latitude
                                                                      ),
                                                           row.getValue(6));
                location->setRelativePopulation(row.getValue<double>("population_rel_to_parent") * populationSizesMapping[parentId]);

                geoGrid->addLocation(location);
        }
}
} // namespace gengeopop
