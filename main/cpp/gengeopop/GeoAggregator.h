#pragma once

#include <tuple>

#include "GeoGrid.h"
#include "KdTree.h"
#include "Location.h"

namespace gengeopop {

/// A functor that aggregates into a vector, if a vector is passed it should remain alive for the usage duration of the
/// Collector
template <typename T>
class Collector
{
public:
        Collector() : m_vec() {}
        Collector(std::vector<T>& vec) : m_vec(vec) {}

        void operator()(T elem) { m_vec.emplace_back(std::move(elem)); }

private:
        std::vector<T>& m_vec;
};

template <typename T>
Collector<T> MakeCollector(std::vector<T>& vec)
{
        return Collector<T>(vec);
}

/// A GeoAggregator can either be instanciated with a functor, or be called with one every time
template <typename Policy, typename... F>
class GeoAggregator
{
        static_assert(sizeof...(F) <= 1, "Should have at most one functor type");
};

/// A GeoAggregator instanciated with a functor
template <typename Policy, typename F>
class GeoAggregator<Policy, F>
{
public:
        GeoAggregator(F f, typename Policy::Args&& args)
            : m_policy(std::forward<typename Policy::Args>(args)), m_functor(std::move(f))
        {
        }

        void operator()()
        {
                // TODO
        }

private:
        Policy m_policy;
        F      m_functor;
};

/// A GeoAggregator that has to be called with a functor
template <typename Policy>
class GeoAggregator<Policy>
{
public:
        GeoAggregator(typename Policy::Args&& args) : m_policy(std::forward<typename Policy::Args>(args)) {}

        template <typename F>
        void operator()(F f)
        {
                // TODO
        }

private:
        Policy m_policy;
};

/// A policy for GeoAggregator that aggregates locations within a radius (in km) of a center point
class RadiusPolicy
{
public:
        using Args = std::tuple<Location, double>;

        RadiusPolicy(const Args& args)
            : m_center(std::get<0>(args).GetCoordinate().longitude, std::get<0>(args).GetCoordinate().latitude),
              m_radius(std::get<1>(args))
        {
        }

        AABB<geogrid_detail::KdTree2DPoint> GetBoundingBox() const;

        bool Contains(const geogrid_detail::KdTree2DPoint& pt) const;

private:
        geogrid_detail::BoostPoint m_center;
        double                     m_radius;
};

} // namespace gengeopop
