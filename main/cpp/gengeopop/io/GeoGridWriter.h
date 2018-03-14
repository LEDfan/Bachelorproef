#pragma once

#include "../GeoGrid.h"
#include <memory>
#include <ostream>

namespace gengeopop {

class GeoGridWriter
{
public:
        virtual void write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream) = 0;
        virtual ~GeoGridWriter(){};
};

} // namespace gengeopop
