#pragma once

#include "CommutesReader.h"
#include <gengeopop/Location.h>
#include <map>
#include <string>
#include <util/CSV.h>

namespace gengeopop {

/**
 * Create a Reader which fills a GeoGrid with the Commutes found in a given CSV file
 */
class CommutesCSVReader : public CommutesReader
{
public:
        /// Construct the CommutesCSVReader with an istream containing the CSV data
        explicit CommutesCSVReader(std::unique_ptr<std::istream> istream);

        /// Add the found Commutes to the provided GeoGrid
        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;

private:
        /// Find the relative total populution in the submunicipalities
        double MunicipalityTotal(std::shared_ptr<Location> loc) const;

        stride::util::CSV m_reader; ///< The pure CSV reader
};
} // namespace gengeopop
