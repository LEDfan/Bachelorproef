#pragma once
#include <gengeopop/io/GeoGridReader.h>
#include <string>

namespace gengeopop {
class GeoGridReaderFactory
{
public:
        std::shared_ptr<GeoGridReader> CreateReader(std::string filename) const;
};
} // namespace gengeopop
