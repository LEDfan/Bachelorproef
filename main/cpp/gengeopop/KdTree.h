#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

namespace {
class BaseNode;
template <typename P, std::size_t D> class Node;
} // namespace

/**********************************
*  Public interface starts here  *
**********************************/

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
 *  - A `get(std::size_t d)` method that returns the coordinate of the `d`th dimension of the point
 *  - The dimensions should have a total order
 */
template <typename P>
class KdTree
{
public:
        /**
         * Constructor: builds an empty tree
         */
        KdTree() : m_size(0), m_root(nullptr) {}

        /**
         * Build a balanced tree from the given set of points efficiently
         *
         * @param points The points to insert in the resulting tree
         * @returns A balanced KdTree containing the given points
         */
        static KdTree Build(const std::vector<P>& points) {
                //TODO
        }

        /**
         * Insert a new point into the tree, using this often may result in an unbalanced tree
         *
         * @param point The point to insert into the tree
         */
        void Insert(P point) {
                //TODO
        }

        /**
         * Test wether a point is contained in the tree
         *
         * @param point The point to test. P should support `bool operator==(const P&) const`
         * @returns Whether the point is found in the tree
         */
        bool Contains(P point) const {
                bool result = false;
                Apply([&result, &point](const P& pt) -> bool {
                        if (pt == point) {
                                result = true;
                                return false;
                        }
                        return true;
                });
        }

        /**
         * Get all points in the tree that lie within `box`
         *
         * @param box The limiting AABB to search for points
         * @returns A collection of points found within `box`
         */
        std::vector<P> Query(const AABB<P>& box) const {
                std::vector<P> result;
                Apply([&result](const P& pt) -> bool {
                        result.push_back(pt);
                }, box);
                return result;
        }

        /**
         * Calls a function with each of the points in the tree
         *
         * @param f A function that will be called with each point, if f returns false, the traversal stops
         */
        void Apply(std::function<bool(const P&)> f) const;

        /**
         * Calls a function with every point contained in `box`
         *
         * @param f A function that will be called with each point within `box`, if f returns false, the traversal stops
         * @param box The containing Axis-Aligned Bounding Box to search for points
         */
        void Apply(std::function<bool(const P&)> f, const AABB<P>& box) const;

        /**
         * Get the height of the tree
         *
         * Mostly for testing purposes
         */
        std::size_t Height() const;

        /**
         * Is the tree empty
         */
        bool Empty() const;

        /**
         * Get the size of the tree
         */
        std::size_t Size() const;

private:

        std::size_t m_size; ///< The number of points in the tree
        std::unique_ptr<Node<P, 0>> m_root; ///< The root node of the tree
};

} // namespace gengeopop



/***************************************
*  Implementation details start here  *
***************************************/
namespace {

class BaseNode {
        virtual ~BaseNode() {};

        /**
         * Get a non-owning pointer to the left child
         * nullptr if there's no such child
         */
        virtual BaseNode* BorrowLeft() = 0;

        /**
         * Get a non-owning pointer to the right child
         * nullptr if there's no such child
         */
        virtual BaseNode* BorrowRight() = 0;
};

/**
 * A node in the KdTree
 *
 * Template parameter P: the type of point
 * Template parameter D: The dimension this node splits on
 */
template <typename P, std::size_t D>
class Node : public BaseNode {
        P point;
        std::unique_ptr<Node<P, (D + 1) % P::dim>> left, right;

        Node(P pt) : point(pt) {}

        BaseNode* BorrowLeft() override;
        BaseNode* BorrowRight() override;
};

} // namespace
