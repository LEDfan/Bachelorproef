#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace gengeopop {

/**
 * Axis Aligned Bounding Box
 *
 * @brief A hyper rectangle defined by 2 points: the lower bound for every dimension and the upper bound
 */
template <typename P>
struct AABB
{
        P lower; ///< The lower bound for every dimension
        P upper; ///< The upper bound for every dimension
};

/**
 * A k-d tree: a k-dimensional generalization of binary search trees
 * This data structure allows for efficient lookup of points and range queries with an Axis-Aligned Bounding Box (when balanced).
 *
 * The template parameter `P` should have the following attributes and operations:
 *  - A `static constexpr std::size_t dim`: the number of dimensions of the point type.
 *  - A `template <std::size_t d> get()` method that returns the coordinate of the `d`th dimension of the point
 */
template <typename P>
class KdTree
{
public:
        /**
         * Constructor: builds an empty tree
         */
        KdTree();

        /**
         * Build a balanced tree from the given set of points efficiently
         *
         * @param points The points to insert in the resulting tree
         * @returns A balanced KdTree containing the given points
         */
        static KdTree Build(const std::vector<P>& points);

        /**
         * Insert a new point into the tree, using this often may result in an unbalanced tree
         *
         * @param point The point to insert into the tree
         */
        void Insert(P point);

        /**
         * Test wether a point is contained in the tree
         *
         * @param point The point to test
         * @returns Whether the point is found in the tree
         */
        bool Contains(P point);

        /**
         * Get all points in the tree that lie within `box`
         *
         * @param box The limiting AABB to search for points
         * @returns A collection of points found within `box`
         */
        std::vector<P> Query(const AABB<P>& box);

        /**
         * Calls a function with each of the points in the tree
         *
         * @param f A function that will be called with each point
         */
        void Apply(std::function<void(const P&)> f);

        /**
         * Calls a function with every point contained in `box`
         *
         * @param f A function that will be called with each point within `box`
         * @param box The containing Axis-Aligned Bounding Box to search for points
         */
        void Apply(std::function<void(const P&)> f, const AABB<P>& box);
};

} // namespace gengeopop
