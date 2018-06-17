#pragma once
#include <gengeopop/io/GeoGridWriter.h>
#include <string>

namespace gengeopop {

/**
 * A Factory for creating the correct implementation of a GeoGridWriter based on the filename extension
 */
class GeoGridWriterFactory
{
public:
        /// Create a GeoGridWriter based on the provided extension in the filename
        std::shared_ptr<GeoGridWriter> CreateWriter(std::string filename) const;
};
} // namespace gengeopop
