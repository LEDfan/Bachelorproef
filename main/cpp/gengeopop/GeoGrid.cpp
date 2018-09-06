/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

#include "GeoGrid.h"
#include <cmath>
#include <iostream>
#include <queue>
#include <util/Exception.h>
#include <utility>

namespace gengeopop {

GeoGrid::GeoGrid()
    : m_locations(), m_locationsToIdIndex(), m_population(stride::Population::Create()), m_finalized(false), m_tree(),
      m_regionId(0), m_regionName("Default")
{
}

GeoGrid::GeoGrid(std::shared_ptr<stride::Population> population, std::size_t regionId, std::string regionName)
    : m_locations(), m_locationsToIdIndex(), m_population(std::move(population)), m_finalized(false), m_tree(),
      m_regionId(regionId), m_regionName(std::move(regionName))

{
}

GeoGrid::iterator GeoGrid::begin() { return m_locations.begin(); }

GeoGrid::iterator GeoGrid::end() { return m_locations.end(); }

void GeoGrid::AddLocation(std::shared_ptr<Location> location)
{
        if (m_finalized) {
                throw stride::util::Exception("Calling addLocation while GeoGrid is finalized is not supported!");
        }

        m_locations.emplace_back(location);
        m_locationsToIdIndex[location->GetID()] = location;
}

std::shared_ptr<Location> GeoGrid::operator[](size_t index) { return *(begin() + index); }
std::shared_ptr<Location>          GeoGrid::Get(size_t index) { return (*this)[index]; }

std::vector<std::shared_ptr<Location>> GeoGrid::TopK(size_t k) const
{
        auto cmp = [](const std::shared_ptr<Location>& rhs, const std::shared_ptr<Location>& lhs) {
                return rhs->GetPopulation() > lhs->GetPopulation();
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
        m_locationsToIdIndex.erase(location->GetID());
}

void GeoGrid::Finalize()
{
        std::vector<geogrid_detail::KdTree2DPoint> points;
        for (auto it = begin(); it != end(); ++it) {
                points.emplace_back(geogrid_detail::KdTree2DPoint(*it));
        }

        m_finalized = true;
        m_tree      = KdTree<geogrid_detail::KdTree2DPoint>::Build(points);
}

std::set<std::shared_ptr<Location>> GeoGrid::InBox(double long1, double lat1, double long2, double lat2) const
{
        CheckFinalized(__func__);

        std::set<std::shared_ptr<Location>> result;
        auto agg = BuildAggregator<BoxPolicy>(MakeCollector(std::inserter(result, result.begin())),
                                              std::make_tuple(std::min(long1, long2), std::min(lat1, lat2),
                                                              std::max(long1, long2), std::max(lat1, lat2)));
        agg();
        return result;
}

std::vector<std::shared_ptr<Location>> GeoGrid::FindLocationsInRadius(std::shared_ptr<Location> start,
                                                                      double                    radius) const
{
        CheckFinalized(__func__);

        geogrid_detail::KdTree2DPoint startPt(start);

        std::vector<std::shared_ptr<Location>> result;
        auto agg = BuildAggregator<RadiusPolicy>(MakeCollector(std::back_inserter(result)),
                                                 std::make_tuple(std::move(startPt), radius));
        agg();
        return result;
}

std::shared_ptr<stride::Population> GeoGrid::GetPopulation() { return m_population; }

void GeoGrid::CheckFinalized(const std::string& functionName) const
{
        if (!m_finalized) {
                throw stride::util::Exception("Calling \"" + functionName +
                                              "\" while GeoGrid is not finalized is not supported!");
        }
}

stride::ContactPool* GeoGrid::CreateContactPool(stride::ContactPoolType::Id type)
{
        return m_population->CreateContactPool(m_regionId, type);
}

std::string GeoGrid::GetRegionName() const { return m_regionName; }

} // namespace gengeopop
