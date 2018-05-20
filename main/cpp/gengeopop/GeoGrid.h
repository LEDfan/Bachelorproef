#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <pop/Population.h>

#include "KdTree.h"
#include "Location.h"

// Trying to include specific headers to lower compilation times
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/within.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/strategies/geographic/distance.hpp>

namespace gengeopop {
template <typename Policy, typename... F>
class GeoAggregator;

namespace geogrid_detail {
using BoostPoint = boost::geometry::model::point<double, 2, boost::geometry::cs::geographic<boost::geometry::degree>>;
class KdTree2DPoint
{
public:
        explicit KdTree2DPoint(const std::shared_ptr<Location>& location)
            : m_pt(location->GetCoordinate().longitude, location->GetCoordinate().latitude), m_location(location)
        {
        }

        KdTree2DPoint() : m_pt(), m_location(nullptr){};

        KdTree2DPoint(double longt, double lat) : m_pt(longt, lat), m_location(nullptr) {}

        constexpr static std::size_t dim = 2;

        template <std::size_t D>
        double Get() const
        {
                static_assert(0 <= D && D <= 1, "Dimension should be in range");
                return boost::geometry::get<D>(m_pt);
        }

        bool InBox(const AABB<KdTree2DPoint>& box) const
        {
                return boost::geometry::within(m_pt,
                                               boost::geometry::model::box<BoostPoint>{box.lower.m_pt, box.upper.m_pt});
        }

        /// Does the point lie within `radius` km from `start`?
        bool InRadius(const KdTree2DPoint& start, double radius) const { return Distance(start) <= radius; }

        std::shared_ptr<Location> GetLocation() const { return m_location; }

        template <std::size_t D>
        struct dimension_type
        {
                using type = double;
        };

        BoostPoint AsBoostPoint() const { return m_pt; }

private:
        BoostPoint                m_pt;
        std::shared_ptr<Location> m_location;

        /// Distance in kilometers, following great circle distance on a speroid earth
        double Distance(const KdTree2DPoint& other) const
        {
                return boost::geometry::distance(m_pt, other.m_pt,
                                                 boost::geometry::strategy::distance::geographic<>{}) /
                       1000.0;
        }
};

} // namespace geogrid_detail

class GeoGrid
{
public:
        using iterator       = std::vector<std::shared_ptr<Location>>::iterator;
        using const_iterator = std::vector<std::shared_ptr<Location>>::const_iterator;

        GeoGrid(std::shared_ptr<stride::Population> population, std::size_t regionId);

        GeoGrid();

        /// Adds a location to this GeoGrid
        void AddLocation(std::shared_ptr<Location> location);

        /// Disables the addLocation method and builds the kdtree.
        void Finalize();

        /// Search for locations in \p radius around \p start
        std::vector<std::shared_ptr<Location>> FindLocationsInRadius(std::shared_ptr<Location> start,
                                                                     double                    radius) const;

        /// Gets the K biggest Location of this GeoGrid
        std::vector<std::shared_ptr<Location>> TopK(size_t k) const;

        /**
         * Gets the locations in a rectangle determined by the two coordinates (long1, lat1) and (long2, lat2).
         *
         * The coordinates must be position on the diagonal, i.e:
         *
         *  p1 -----+     +-------p1
         *  |       |     |       |
         *  |       |  or |       |
         *  |       |     |       |
         *  +-------p2    p2------+
         */
        std::set<std::shared_ptr<Location>> InBox(double long1, double lat1, double long2, double lat2) const;

        /// Gets the location in a rectangle defined by the two locations
        std::set<std::shared_ptr<Location>> InBox(const std::shared_ptr<Location>& loc1,
                                                  const std::shared_ptr<Location>& loc2) const
        {
                return InBox(loc1->GetCoordinate().longitude, loc1->GetCoordinate().latitude,
                             loc2->GetCoordinate().longitude, loc2->GetCoordinate().latitude);
        }

        /// Iterator to first Location
        iterator begin();

        /// Iterator to the end of the Location storage
        iterator end();

        /// Const Iterator to first Location
        const_iterator cbegin() const;

        /// Const iterator to the end of the Location storage
        const_iterator cend() const;

        /// Gets amount of Location
        size_t size() const;

        /// Remove element of GeoGrid
        void remove(const std::shared_ptr<Location>& location);

        /// Gets a Location by index, doesn't performs a range check
        std::shared_ptr<Location> operator[](size_t index);

        /// Gets a Location by index, doesn't performs a range check
        std::shared_ptr<Location> Get(size_t index);

        /// Gets a Location by id and check if the id exists
        std::shared_ptr<Location> GetById(unsigned int id);

        /// Create and store a Person in the GeoGrid and return a pointer to it, which works until deletion of the
        /// GeoGrid
        template <typename... Args>
        stride::Person* CreatePerson(Args&&... args)
        {
                m_population->CreatePerson(m_regionId, args...);
                return &m_population->back(m_regionId);
        }

        /// Get the population of this GeoGrid
        std::shared_ptr<stride::Population> GetPopulation();

        template <typename Policy, typename F>
        GeoAggregator<Policy, F> BuildAggregator(F functor, typename Policy::Args&& args) const;

        template <typename Policy>
        GeoAggregator<Policy> BuildAggregator(typename Policy::Args&& args) const;

private:
        void CheckFinalized(const std::string& functionName)
            const; ///< Checks whether the GeoGrid is finalized and thus certain operations can(not) be used

        std::vector<std::shared_ptr<Location>> m_locations; ///< Locations in this geoGrid
        std::unordered_map<unsigned int, std::shared_ptr<Location>>
                                            m_locationsToIdIndex; ///< Locations in this geoGrid indexed by Id
        std::shared_ptr<stride::Population> m_population;         ///< Stores and Owns the population of this GeoGrid

        bool m_finalized;

        KdTree<geogrid_detail::KdTree2DPoint> m_tree;

        std::size_t m_regionId; ///< RegionId, used to create persons
};

} // namespace gengeopop

#include "GeoAggregator.h" // Prevent cyclic include dependency

namespace gengeopop {

template <typename Policy, typename F>
GeoAggregator<Policy, F> GeoGrid::BuildAggregator(F functor, typename Policy::Args&& args) const
{
        return GeoAggregator<Policy, F>(m_tree, functor, std::forward<typename Policy::Args>(args));
}

template <typename Policy>
GeoAggregator<Policy> GeoGrid::BuildAggregator(typename Policy::Args&& args) const
{
        return GeoAggregator<Policy>(m_tree, std::forward<typename Policy::Args>(args));
}

} // namespace gengeopop
