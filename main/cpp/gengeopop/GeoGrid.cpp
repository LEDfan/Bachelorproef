#include "GeoGrid.h"
#include <cmath>
#include <iostream>
#include <queue>

namespace gengeopop {

GeoGrid::GeoGrid() : m_locations(), m_locationsToIdIndex(), m_finalized(false), m_tree() {}

GeoGrid::iterator GeoGrid::begin() { return m_locations.begin(); }

GeoGrid::iterator GeoGrid::end() { return m_locations.end(); }

void GeoGrid::addLocation(std::shared_ptr<Location> location)
{
        if (m_finalized) {
                throw std::runtime_error("Calling addLocation while GeoGrid is finalized is not supported!");
        }

        m_locations.emplace_back(location);
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

void GeoGrid::remove(const std::shared_ptr<Location>& location)
{
        m_locations.erase(std::remove(m_locations.begin(), m_locations.end(), location), m_locations.end());
        m_locationsToIdIndex.erase(location->getID());
}

void GeoGrid::finalize()
{
        std::vector<KdTree2DPoint> points;
        for (auto it = begin(); it != end(); ++it) {
                points.emplace_back(KdTree2DPoint(*it));
        }

        m_finalized = true;
        m_tree      = KdTree<KdTree2DPoint>::Build(points);
}

std::set<std::shared_ptr<Location>> GeoGrid::inBox(double long1, double lat1, double long2, double lat2) const
{
        if (!m_finalized) {
                throw std::runtime_error("Calling inBox while GeoGrid is not finalized is not supported!");
        }

        std::set<std::shared_ptr<Location>> result;

        m_tree.Apply(
            [&result](const KdTree2DPoint& pt) -> bool {
                    result.insert(pt.getLocation());
                    return true;
            },
            {{std::min(long1, long2), std::min(lat1, lat2)}, {std::max(long1, long2), std::max(lat1, lat2)}});
        return result;
}

std::set<std::shared_ptr<Location>> GeoGrid::findLocationsInRadius(std::shared_ptr<Location> start, double radius) const
{
        AABB<KdTree2DPoint> box{};

        double maxlat = start->getCoordinate().latitude + radianToDegree(radius / 6371.0);
        double minlat = start->getCoordinate().latitude - radianToDegree(radius / 6371.0);

        double maxlon = start->getCoordinate().longitude +
                        radianToDegree(radius / 6371.0 / std::cos(degreeToRadian(start->getCoordinate().latitude)));
        double minlon = start->getCoordinate().longitude -
                        radianToDegree(radius / 6371.0 / std::cos(degreeToRadian(start->getCoordinate().latitude)));

        box.upper = KdTree2DPoint(maxlon, maxlat);
        box.lower = KdTree2DPoint(minlon, minlat);

        KdTree2DPoint startPt(start);

        std::set<std::shared_ptr<Location>> result;

        m_tree.Apply(
            [&startPt, &radius, &result](const KdTree2DPoint& pt) -> bool {
                    if (pt.InRadius(startPt, radius)) {
                            result.insert(pt.getLocation());
                    }
                    return true;
            },
            box);

        return result;
}

std::function<std::shared_ptr<stride::Person>(unsigned int, double, unsigned int, unsigned int, unsigned int,
                                              unsigned int, unsigned int)> GeoGrid::createPersonImpl = [](unsigned int id, double age, unsigned int household_id, unsigned int school_id, unsigned int work_id,
                                                          unsigned int primary_community_id, unsigned int secondary_community_id) -> std::shared_ptr<stride::Person>
{
        return std::make_shared<stride::Person>(id, age, household_id, school_id, work_id, primary_community_id, secondary_community_id);
};

} // namespace gengeopop
