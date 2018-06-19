#pragma once
#include <gengeopop/io/GeoGridReader.h>
#include <string>

namespace gengeopop {

/**
 * A Factory for creating the correct implementation of a GeoGridReader based on the filename extension
 */
class GeoGridReaderFactory
{
public:
        /// Create a GeoGridReader based on the provided extension in the filename
        std::shared_ptr<GeoGridReader> CreateReader(std::string filename) const;
};
} // namespace gengeopop
