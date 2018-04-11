#include "SubMunicipalitiesCSVReader.h"
#include "../../util/CSV.h"
#include "../../util/CSVRow.h"
#include <iostream>

namespace gengeopop {

SubMunicipalitiesCSVReader::SubMunicipalitiesCSVReader(std::unique_ptr<std::istream> inputStream)
    : SubMunicipalitiesReader(std::move(inputStream))
{
}

void SubMunicipalitiesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{
        // columns: parent_id,id,parent_is_sub,population_rel_to_parent,latitude,longitude,name
        stride::util::CSV reader(*(m_inputStream.get()));

        // first build a list of relative population sizes of each Location
        std::map<unsigned int, double> populationSizesMapping;

        for (const auto& loc : *geoGrid) {
                populationSizesMapping[loc->getID()] = loc->getRelativePopulationSize();
        }

        for (const stride::util::CSVRow& row : reader) {
                auto parentId = row.getValue<unsigned int>(0);
                auto id       = row.getValue<unsigned int>(1);
                auto parent   = geoGrid->GetById(parentId);

                parent->setRelativePopulation(0);
                auto location = std::make_shared<Location>(id,                                           // id
                                                           parent->getProvince(),                        // province
                                                           Coordinate(0,                                 // x_coord
                                                                      0,                                 // y_coord
                                                                      row.getValue<double>("longitude"), // longtitude
                                                                      row.getValue<double>("latitude")   // latitude
                                                                      ),
                                                           row.getValue(5));
                location->setRelativePopulation(row.getValue<double>("population_rel_to_parent") *
                                                populationSizesMapping[parentId]);
                parent->addSubMunicipality(location);
                location->setParent(parent);
                geoGrid->addLocation(location);
        }
}
} // namespace gengeopop
