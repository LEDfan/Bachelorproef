#pragma  once

#include <string>
#include <gengeopop/Location.h>

namespace gengeopop {
    class CitiesReader {
    public:
        std::vector<std::shared_ptr<Location> > getLocations();

    protected:
        std::vector<std::shared_ptr<Location> > m_locations;
    };
}

