#pragma once

#include "CitiesReader.h"
#include <util/CSV.h>

namespace gengeopop {

/**
 * Create a Reader which fills a GeoGrid with the Cities found in a given CSV file
 */
class CitiesCSVReader : public CitiesReader
{
public:
        /// Construct the CitiesCSVReader with an istream containing the CSV data
        explicit CitiesCSVReader(std::unique_ptr<std::istream> inputStream);

        /// Add the found Locations to the provided GeoGrid
        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;

private:
        stride::util::CSV m_reader; ///< The pure CSV reader
};
} // namespace gengeopop
