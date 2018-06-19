#include "CitiesCSVReader.h"
#include "../../util/CSV.h"
#include "../../util/CSVRow.h"

namespace gengeopop {

CitiesCSVReader::CitiesCSVReader(std::unique_ptr<std::istream> inputStream)
    : CitiesReader(std::move(inputStream)), m_reader(*(m_inputStream.get()))
{
}

void CitiesCSVReader::FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const
{
        // cols: id 	province 	population 	x_coord 	y_coord 	latitude 	longitude
        // name
        std::vector<std::pair<std::shared_ptr<Location>, int>> addedLocations;

        unsigned int totalPopulation = 0;

        for (const stride::util::CSVRow& row : m_reader) {
                auto id       = row.GetValue<int>(0);
                auto location = std::make_shared<Location>(id,                   // id
                                                           row.GetValue<int>(1), // province
                                                           // ignore x and y, not sure what they actually do
                                                           Coordinate(row.GetValue<double>(6), // longtitude
                                                                      row.GetValue<double>(5)  // latitude
                                                                      ),
                                                           row.GetValue(7));

                geoGrid->AddLocation(location);
                addedLocations.emplace_back(location, row.GetValue<int>(2));
                totalPopulation += row.GetValue<int>(2);
        }

        for (const auto& loc : addedLocations) {
                loc.first->SetRelativePopulation(static_cast<double>(loc.second) /
                                                 static_cast<double>(totalPopulation));
        }
}
} // namespace gengeopop
