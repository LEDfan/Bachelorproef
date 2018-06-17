#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
/**
 * Create an abstract Reader which fills a GeoGrid with the SubMunicipalities found in a given file.
 * This can be implemented using different input file types. Currently CSV is implemented.
 */
class SubMunicipalitiesReader
{
public:
        /// Construct the SubMunicipalitiesReader with an istream containing the file content
        explicit SubMunicipalitiesReader(std::unique_ptr<std::istream> inputStream);

        /// Add the found SubMunicipalities to the provided GeoGrid
        virtual void FillGeoGrid(std::shared_ptr<GeoGrid>) const = 0;

        /// Default constructor
        virtual ~SubMunicipalitiesReader() = default;

protected:
        std::unique_ptr<std::istream> m_inputStream; ///< The istream containing the file content
};
} // namespace gengeopop
