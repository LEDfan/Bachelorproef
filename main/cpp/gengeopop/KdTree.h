#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace {
template <typename P>
class BaseNode;
template <typename P, std::size_t D>
class Node;
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
 * This data structure allows for efficient lookup of points and range queries with an Axis-Aligned Bounding Box (when
 * balanced).
 *
 * The template parameter `P` should have the following attributes and operations:
 *  - A `static constexpr std::size_t dim`: the number of dimensions of the point type.
 *  - A `get(std::size_t d)` method that returns the coordinate of the `d`th dimension of the point
 *  - A default constructor and a copy constructor
 *  - The individual dimensions should each have a total order and equality
 */
template <typename P>
class KdTree
{
public:
        static_assert(P::dim > 0, "Cannot have points in 0 dimensions");

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
        static KdTree Build(const std::vector<P>& points)
        {
                KdTree result;
                result.m_size = points.size();
                result.m_root = Construct<0>(points);
                return result;
        }

        /**
         * Insert a new point into the tree, using this often may result in an unbalanced tree
         *
         * @param point The point to insert into the tree
         */
        void Insert(P point)
        {
                m_size++;
                if (!m_root) {
                        m_root = std::make_unique<Node<P, 0>>(point);
                        return;
                }
                BaseNode<P>* current = m_root.get();
                while (true) {
                        BaseNode<P>* next = current->BorrowSplitChild(point);
                        if (!next) {
                                current->AddChild(point);
                                return;
                        }
                        current = next;
                }
        }

        /**
         * Test wether a point is contained in the tree
         *
         * @param point The point to test. P should support `bool operator==(const P&) const`
         * @returns Whether the point is found in the tree
         */
        bool Contains(P point) const
        {
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
        std::vector<P> Query(const AABB<P>& box) const
        {
                std::vector<P> result;
                Apply(
                    [&result](const P& pt) -> bool {
                            result.push_back(pt);
                            return true;
                    },
                    box);
                return result;
        }

        /**
         * Calls a function with each of the points in the tree
         *
         * @param f A function that will be called with each point, if f returns false, the traversal stops
         */
        void Apply(std::function<bool(const P&)> f) const
        {
                if (!m_root)
                        return;

                std::queue<BaseNode<P>*> todo;
                todo.push(m_root.get());

                while (!todo.empty()) {
                        BaseNode<P>* current = todo.top();
                        todo.pop();

                        f(current->GetPoint());

                        BaseNode<P>* left = current->BorrowLeft();
                        if (left)
                                todo.push(left);

                        BaseNode<P>* right = current->BorrowRight();
                        if (right)
                                todo.push(right);
                }
        }

        /**
         * Calls a function with every point contained in `box`
         *
         * @param f A function that will be called with each point within `box`, if f returns false, the traversal stops
         * @param box The containing Axis-Aligned Bounding Box to search for points
         */
        void Apply(std::function<bool(const P&)> f, const AABB<P>& box) const
        {
                std::queue<BaseNode<P>*> q;
                q.push(m_root.get());
                while (!q.empty()) {
                        BaseNode<P>* current = q.front();
                        q.pop();
                        if (!current || !current->InBox(box))
                                continue;

                        f(current->GetPoint());

                        q.push(current->BorrowLeft());
                        q.push(current->BorrowRight());
                }
        }

        /**
         * Get the height of the tree
         *
         * Mostly for testing purposes
         */
        std::size_t Height() const
        {
                int                                      h = 0;
                std::queue<std::pair<int, BaseNode<P>>*> q;
                q.emplace(1, m_root.get());
                while (!q.empty()) {
                        auto tmp = q.front();
                        q.pop();
                        BaseNode<P>* n = tmp.second;
                        if (!n)
                                continue;
                        h = tmp.first;
                        q.emplace(h + 1, n);
                }
                return h;
        }

        /**
         * Is the tree empty
         */
        bool Empty() const { return Size() == 0; }

        /**
         * Get the size of the tree
         */
        std::size_t Size() const { return m_size; }

private:
        template <std::size_t D>
        static std::unique_ptr<Node<P, D>> Construct(const std::vector<P>& points)
        {
                if (points.empty())
                        return nullptr;

                namespace BA = boost::accumulators;
                BA::accumulator_set<decltype(points[0].template get<D>()), BA::features<BA::tag::median>> acc;
                for (const P& p : points)
                        acc(p.template get<D>());

                auto median = BA::median(acc);
                bool taken  = false;
                P    root_pt;

                std::vector<P> left, right;
                for (const P& p : points) {
                        if (!taken && p.template get<D>() == median) {
                                taken   = true;
                                root_pt = p;
                        } else if (p.template get<D>() <= median) {
                                left.push_back(p);
                        } else {
                                right.push_back(p);
                        }
                }

                auto root   = std::make_unique<Node<P, D>>(root_pt);
                root->left  = Construct<(D + 1) % P::dim>(left);
                root->right = Construct<(D + 1) % P::dim>(right);

                return root;
        }

        std::size_t                 m_size; ///< The number of points in the tree
        std::unique_ptr<Node<P, 0>> m_root; ///< The root node of the tree
};

/***************************************
 *  Implementation details start here  *
 ***************************************/
namespace {

template <typename P>
class BaseNode
{
public:
        virtual ~BaseNode(){};

        /**
         * Get a non-owning pointer to the left child
         * nullptr if there's no such child
         */
        virtual BaseNode<P>* BorrowLeft() const = 0;

        /**
         * Get a non-owning pointer to the right child
         * nullptr if there's no such child
         */
        virtual BaseNode<P>* BorrowRight() const = 0;

        /**
         * Get a non-owning pointer to the child corresponding to the correct split for point
         */
        virtual BaseNode<P>* BorrowSplitChild(const P& point) const = 0;

        /**
         * Add a new child in the right place, according to split
         */
        virtual void AddChild(P point) = 0;

        /**
         * Gets the point for this node
         */
        virtual P GetPoint() const = 0;

        /**
         * Test wether this node falls within the Axis-Aligned Bounding Box for its own dimension
         */
        virtual bool InBox(const AABB<P>& box) const = 0;
};

/**
 * A node in the KdTree
 *
 * Template parameter P: the type of point
 * Template parameter D: The dimension this node splits on
 */
template <typename P, std::size_t D>
class Node : public BaseNode<P>
{
public:
        Node(P pt) : m_point(pt) {}

        BaseNode<P>* BorrowLeft() const override { return m_left.get(); }

        BaseNode<P>* BorrowRight() const override { return m_right.get(); }

        BaseNode<P>* BorrowSplitChild(const P& point) const override
        {
                auto refval  = m_point.template get<D>();
                auto testval = point.template get<D>();
                if (testval <= refval) {
                        return m_left.get();
                } else {
                        return m_right.get();
                }
        }

        void AddChild(P point) override
        {
                auto refval  = m_point.template get<D>();
                auto testval = point.template get<D>();
                if (testval <= refval) {
                        m_left = std::make_unique<Child>(point);
                } else {
                        m_left = std::make_unique<Child>(point);
                }
        }

        P GetChild() const override { return m_point; }

        bool InBox(const AABB<P>& box) const override
        {
                auto val = m_point.template get<D>();
                return (box.lower.template get<D>() <= val && val <= box.upper.template get<D>());
        }

private:
        using Child = Node<P, (D + 1) % P::dim>;

        P                      m_point;
        std::unique_ptr<Child> m_left, m_right;
};

} // namespace
} // namespace gengeopop
