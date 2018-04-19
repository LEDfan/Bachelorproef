#pragma once
#include <gengeopop/io/GeoGridWriter.h>
#include <string>

namespace gengeopop {
class GeoGridWriterFactory
{
public:
        std::shared_ptr<GeoGridWriter> createWriter(std::string filename) const;
};
} // namespace gengeopop
