#pragma  once

#include <string>
#include <gengeopop/Location.h>

namespace gengeopop {
    class CitiesReader {
    public:
            CitiesReader();
        const std::vector<std::shared_ptr<Location> >& getLocations() const;

    protected:
        std::vector<std::shared_ptr<Location> > m_locations;
    };
}

