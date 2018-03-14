#include "CitiesReader.h"

namespace gengeopop {

CitiesReader::CitiesReader() : m_locations() {}

const std::map<int, std::shared_ptr<Location>>& CitiesReader::getLocations() const {
        return m_locations;
}
} // namespace gengeopop
