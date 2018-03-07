#pragma once

#include <vector>

#include "Location.h"

namespace gengeopop {

    class GeoGrid {
    public:
    private:
        std::vector<std::shared_ptr<Location> > m_locations;

    };
};

