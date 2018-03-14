#pragma once

#include <memory>
#include <ostream>
#include "../GeoGrid.h"

namespace gengeopop {

    class GeoGridWriter {
    public:
        virtual void write(std::shared_ptr<GeoGrid> geoGrid, std::ostream &stream)=0;
        virtual ~GeoGridWriter() {};
    };

}

