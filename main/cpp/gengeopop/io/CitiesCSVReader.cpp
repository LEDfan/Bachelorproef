#include "CitiesCSVReader.h"
#include "../../util/CSV.h"
#include "../../util/CSVRow.h"

namespace gengeopop {

CitiesCSVReader::CitiesCSVReader(std::unique_ptr<std::istream> inputStream) : CitiesReader(std::move(inputStream)) {}

void CitiesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{
        // cols: id 	province 	population 	x_coord 	y_coord 	latitude 	longitude
        // name
        stride::util::CSV                                      reader(*(m_inputStream.get()));
        std::vector<std::pair<std::shared_ptr<Location>, int>> addedLocations;

        unsigned int totalPopulation = 0;

        for (const stride::util::CSVRow& row : reader) {
                auto id       = row.getValue<int>(0);
                auto location = std::make_shared<Location>(id,                                 // id
                                                           row.getValue<int>(1),               // province
                                                           Coordinate(row.getValue<double>(3), // x_coord
                                                                      row.getValue<double>(4), // y_coord
                                                                      row.getValue<double>(6), // longtitude
                                                                      row.getValue<double>(5)  // latitude
                                                                      ),
                                                           row.getValue(7));
                geoGrid->addLocation(location);
                addedLocations.emplace_back(location, row.getValue<int>(2));
                totalPopulation += row.getValue<int>(2);
        }

        for (const auto& loc : addedLocations) {
                loc.first->setRelativePopulation(static_cast<double>(loc.second) /
                                                 static_cast<double>(totalPopulation));
        }
}
} // namespace gengeopop
