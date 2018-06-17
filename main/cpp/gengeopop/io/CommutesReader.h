#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {

/**
 * Create an abstract Reader which fills a GeoGrid with the Commutes found in a given file.
 * This can be implemented using different input file types. Currently CSV is implemented.
 */
class CommutesReader
{
public:
        /// Construct the CommutesReader with an istream containing the file content
        explicit CommutesReader(std::unique_ptr<std::istream> inputStream);

        /// Add the found Commutes to the provided GeoGrid
        virtual void FillGeoGrid(std::shared_ptr<GeoGrid>) const = 0;

        /// Default destructor
        virtual ~CommutesReader() = default;

protected:
        std::unique_ptr<std::istream> m_inputStream; ///< The istream containing the file content
};
} // namespace gengeopop
