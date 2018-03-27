#include "GeoGrid.h"
#include <cmath>
#include <iostream>
#include <queue>

namespace gengeopop {

GeoGrid::GeoGrid() : m_locations(), m_locationsToIdIndex(), m_finalized(false), m_points(), m_tree() {}

GeoGrid::iterator GeoGrid::begin() { return m_locations.begin(); }

GeoGrid::iterator GeoGrid::end() { return m_locations.end(); }

void GeoGrid::addLocation(std::shared_ptr<Location> location)
{
        if (m_finalized) {
                throw std::runtime_error("Calling addLocation while GeoGrid is finalized is not supported!");
        }

        m_locations.push_back(location);
        m_points.emplace_back(KdTree2DPoint(location));
        m_locationsToIdIndex[location->getID()] = location;
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

std::shared_ptr<Location> GeoGrid::GetById(unsigned int id) { return m_locationsToIdIndex.at(id); }

unsigned int GeoGrid::getTotalPopulation() const
{
        unsigned total = 0;
        for (auto loc = cbegin(); loc < cend(); loc++) {
                total += (*loc)->getPopulation();
        }
        return total;
}

void GeoGrid::finalize()
{
        m_finalized = true;
        m_tree      = KdTree<KdTree2DPoint>::Build(m_points);
}

std::vector<std::shared_ptr<Location>> GeoGrid::inBox(double long1, double lat1, double long2, double lat2) const
{
        if (!m_finalized) {
                throw std::runtime_error("Calling inBox while GeoGrid is not finalized is not supported!");
        }

        std::vector<std::shared_ptr<Location>> result;

        m_tree.Apply(
            [&result](const KdTree2DPoint& pt) -> bool {
                    result.push_back(pt.getLocation());
                    return true;
            },
            {{std::min(long1, long2), std::min(lat1, lat2)}, {std::max(long1, long2), std::max(lat1, lat2)}});
        return result;
}

} // namespace gengeopop
