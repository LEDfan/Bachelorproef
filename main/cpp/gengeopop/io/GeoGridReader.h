#pragma once

#include "../GeoGrid.h"
#include <memory>
#include <ostream>

namespace gengeopop {

class GeoGridReader
{
public:
        virtual ~GeoGridReader(){};
        virtual std::shared_ptr<GeoGrid> read(std::istream& stream) = 0;
};

} // namespace gengeopop
