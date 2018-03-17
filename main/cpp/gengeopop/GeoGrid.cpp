#include "GeoGrid.h"
#include <queue>

namespace gengeopop {

GeoGrid::GeoGrid() : m_locations() {}

GeoGrid::iterator GeoGrid::begin() { return m_locations.begin(); }

GeoGrid::iterator GeoGrid::end() { return m_locations.end(); }

void GeoGrid::addLocation(std::shared_ptr<Location> location) {
        m_locations.push_back(location);
        m_locationsToIdIndex.insert({location->getID(), location});
}

std::shared_ptr<Location> GeoGrid::operator[](size_t index)
{
        // TODO range check needed?
        return *(begin() + index);
}
std::shared_ptr<Location> GeoGrid::get(size_t index) { return (*this)[index]; }

std::vector<std::shared_ptr<Location>> GeoGrid::topK(size_t k) const
{

        auto cmp = [](const std::shared_ptr<Location>& rhs, const std::shared_ptr<Location>& lhs) {
                return rhs->getPopulation() > lhs->getPopulation();
        };

        std::priority_queue<std::shared_ptr<Location>, std::vector<std::shared_ptr<Location>>, decltype(cmp)> queue(
            cmp);

        for (auto it = cbegin(); it != cend(); it++) {
                queue.push(*it);
                if (queue.size() > k) {
                        queue.pop();
                }
        }

        std::vector<std::shared_ptr<Location>> topLocations;

        while (!queue.empty()) {
                auto loc = queue.top();
                topLocations.push_back(loc);
                queue.pop();
        }

        return topLocations;
}

GeoGrid::const_iterator GeoGrid::cbegin() const { return m_locations.cbegin(); }

GeoGrid::const_iterator GeoGrid::cend() const { return m_locations.cend(); }

size_t GeoGrid::size() const { return m_locations.size(); }

std::shared_ptr<Location> GeoGrid::getById(unsigned int id) {
        return m_locationsToIdIndex.at(id);
}

} // namespace gengeopop
