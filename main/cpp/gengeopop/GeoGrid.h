#pragma once

#include <vector>

#include "Location.h"

namespace gengeopop {

    using GeoGridIterator = std::vector<std::shared_ptr<Location>>::iterator;
    class GeoGrid {
    public:
        
        void addLocation(std::shared_ptr<Location> location);

        std::vector<std::shared_ptr<Location>> findNearLocations(std::shared_ptr<Location> start, double range) const;


        GeoGridIterator begin();
        GeoGridIterator end();

    private:
        std::vector<std::shared_ptr<Location> > m_locations;

    };
};

