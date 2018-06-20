#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {

/**
 * Create an abstract Reader which fills a GeoGrid with the Cities found in a given file.
 * This can be implemented using different input file types. Currently CSV is implemented.
 */
class CitiesReader
{
public:
        /// Construct the CitiesReader with an istream containing the file content
        explicit CitiesReader(std::unique_ptr<std::istream> inputStream);

        /// Add the found Locations to the provided GeoGrid
        virtual void FillGeoGrid(std::shared_ptr<GeoGrid>) const = 0;

        /// Default destructor
        virtual ~CitiesReader() = default;

protected:
        std::unique_ptr<std::istream> m_inputStream; ///< The istream containing the file content
};
} // namespace gengeopop
