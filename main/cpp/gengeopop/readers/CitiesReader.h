#pragma  once

#include <string>
#include <gengeopop/Location.h>

namespace gengeopop {
    class CitiesReader {
    public:
        std::vector<std::shared_ptr<Location> > getLocations();
    private:
        std::vector<std::shared_ptr<Location> > m_locations;
    };
}

