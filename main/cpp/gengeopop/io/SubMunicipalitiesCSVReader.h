#pragma once

#include "SubMunicipalitiesReader.h"
#include <util/CSV.h>

namespace gengeopop {

/**
 * Create a Reader which fills a GeoGrid with the SubMunicipalities found in a given CSV file
 */
class SubMunicipalitiesCSVReader : public SubMunicipalitiesReader
{
public:
        /// Construct the SubMunicipalitiesCSVReader with an istream containing the CSV data
        explicit SubMunicipalitiesCSVReader(std::unique_ptr<std::istream> inputStream);

        /// Add the found SubMunicipalities to the provided GeoGrid
        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;

private:
        stride::util::CSV m_reader; ///< The pure CSV reader
};
} // namespace gengeopop
