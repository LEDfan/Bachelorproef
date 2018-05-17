#pragma once

#include <tuple>

#include "GeoGrid.h"
#include "KdTree.h"
#include "Location.h"

namespace geoaggregator_detail {
inline double RadianToDegree(double rad) { return rad / M_PI * 180.0; }

inline double DegreeToRadian(double deg) { return deg / 180.0 * M_PI; }

} // namespace geoaggregator_detail

namespace gengeopop {

/// A functor that aggregates into a vector, if a vector is passed it should remain alive for the usage duration of the
/// Collector
template <typename InsertIter, typename T>
class Collector
{
public:
        Collector(const InsertIter& ins) : m_ins(ins) {}

        void operator()(T elem) { *m_ins = std::move(elem); }

private:
        InsertIter m_ins;
};

template <typename InsertIter, typename T = typename InsertIter::container_type::value_type>
Collector<InsertIter, T> MakeCollector(const InsertIter& ins)
{
        return Collector<InsertIter, T>(ins);
}

/// A GeoAggregator can either be instanciated with a functor, or be called with one every time
template <typename Policy, typename... F>
class GeoAggregator
{
        static_assert(sizeof...(F) <= 1, "Should have at most one functor type");
};

/// A GeoAggregator that has to be called with a functor
template <typename Policy>
class GeoAggregator<Policy>
{
public:
        GeoAggregator(const KdTree<geogrid_detail::KdTree2DPoint>& tree, typename Policy::Args&& args)
            : m_policy(std::forward<typename Policy::Args>(args)), m_tree(tree)
        {
        }

        template <typename F>
        void operator()(F f)
        {
                auto box = m_policy.GetBoundingBox();
                m_tree.Apply(
                    [&f, this](const geogrid_detail::KdTree2DPoint& pt) -> bool {
                            if (m_policy.Contains(pt))
                                    f(pt.GetLocation());
                            return true;
                    },
                    box);
        }

private:
        Policy                                       m_policy;
        const KdTree<geogrid_detail::KdTree2DPoint>& m_tree;
};

/// A GeoAggregator instanciated with a functor
template <typename Policy, typename F>
class GeoAggregator<Policy, F> : public GeoAggregator<Policy>
{
public:
        GeoAggregator(const KdTree<geogrid_detail::KdTree2DPoint>& tree, F f, typename Policy::Args&& args)
            : GeoAggregator<Policy>(tree, std::forward<typename Policy::Args&&>(args)), m_functor(std::move(f))
        {
        }

        void operator()() { GeoAggregator<Policy>::operator()(m_functor); }

private:
        F m_functor;
};

/// A policy for GeoAggregator that aggregates locations within a radius (in km) of a center point
class RadiusPolicy
{
public:
        using Args = std::tuple<geogrid_detail::KdTree2DPoint, double>;

        RadiusPolicy(Args args) : m_center(std::move(std::get<0>(args))), m_radius(std::get<1>(args)) {}

        AABB<geogrid_detail::KdTree2DPoint> GetBoundingBox() const
        {
                using namespace geoaggregator_detail;

                AABB<geogrid_detail::KdTree2DPoint> box{};

                // As of boost 1.66, there's seems no way to do this in Boost.Geometry
                constexpr double EARTH_RADIUS_KM = 6371.0;
                double           scaled_radius   = m_radius / EARTH_RADIUS_KM;

                double startlon = m_center.Get<0>();
                double startlat = m_center.Get<1>();
                double londiff  = RadianToDegree(scaled_radius / std::cos(DegreeToRadian(startlat)));
                double latdiff  = RadianToDegree(scaled_radius);

                box.upper = geogrid_detail::KdTree2DPoint(startlon + londiff, startlat + latdiff);
                box.lower = geogrid_detail::KdTree2DPoint(startlon - londiff, startlat - latdiff);

                return box;
        }

        bool Contains(const geogrid_detail::KdTree2DPoint& pt) const { return pt.InRadius(m_center, m_radius); }

private:
        geogrid_detail::KdTree2DPoint m_center;
        double                        m_radius;
};

/// A GeoAggregator Policy that aggregates over an axis aligned bounding box
class BoxPolicy
{
public:
        using Args = std::tuple<double, double, double, double>; ///< lon1, lat1, lon2, lat2

        BoxPolicy(Args args) : m_args(std::move(args)) {}

        AABB<geogrid_detail::KdTree2DPoint> GetBoundingBox() const
        {
                using std::get;
                return {{get<0>(m_args), get<1>(m_args)}, {get<2>(m_args), get<3>(m_args)}};
        }

        bool Contains(const geogrid_detail::KdTree2DPoint&) const { return true; }

private:
        Args m_args;
};

} // namespace gengeopop