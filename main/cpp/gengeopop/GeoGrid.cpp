#include "GeoGrid.h"

namespace gengeopop {

    GeoGridIterator GeoGrid::begin() {
        return m_locations.begin();
    }

    GeoGridIterator GeoGrid::end() {
        return m_locations.end();
    }

    void GeoGrid::addLocation(std::shared_ptr<Location> location) {
        m_locations.push_back(location);
    }

    std::shared_ptr<Location> GeoGrid::operator[](size_t index) {
        // TODO range check needed?
        return *(begin() + index);
    }

}
