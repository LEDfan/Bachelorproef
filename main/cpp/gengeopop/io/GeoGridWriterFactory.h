#pragma once
#include <gengeopop/io/GeoGridWriter.h>
#include <string>

namespace gengeopop {
class GeoGridWriterFactory
{
public:
        std::shared_ptr<GeoGridWriter> CreateWriter(std::string filename) const;
};
} // namespace gengeopop
