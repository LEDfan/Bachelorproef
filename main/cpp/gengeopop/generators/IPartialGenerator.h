#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/GeoGridConfig.h>

namespace gengeopop {
    /**
     * An interface for generators that provide a partial solution. They generate some data and apply it onto the GeoGrid.
     */
    class IPartialGenerator {
    public:
        virtual void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) = 0;
        virtual ~IPartialGenerator() {};
    };
}
