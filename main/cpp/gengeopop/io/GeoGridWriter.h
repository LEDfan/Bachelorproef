#pragma once

#include "../GeoGrid.h"
#include <memory>
#include <ostream>

namespace gengeopop {

/**
 * An interface for writing the GeoGrid to a file, can be implemented with multiple file types
 * (protobuf and json are currently implemented)
 */
class GeoGridWriter
{
public:
        /// Construct the Writer
        virtual ~GeoGridWriter() {}

        /// Write the provided geoGrid to the provided ostream
        virtual void Write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream) = 0;
};

} // namespace gengeopop
