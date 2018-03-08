#pragma once

#include <gengeopop/GeoGrid.h>

namespace gengeopop {
    /**
     * An interface for generators that provide a partial solution. They generate some data and apply it onto the GeoGrid.
     */
    class IPartialGenerator {
    public:
        void apply(GeoGrid& geogrid) = 0;
    };
}