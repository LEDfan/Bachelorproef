#include <gengeopop/KdTree.h>
#include <cmath>
#include <string>
#include <gtest/gtest.h>

using namespace gengeopop;

namespace {

class Pt2D {
public:
        constexpr static std::size_t dim = 2;

        int x, y;

        template <std::size_t D>
        int get() const {
                static_assert(0 <= D && D <= 1, "Dimension should be in range");
                if (D == 0) {
                        return x;
                } else {
                        return y;
                }
        }
};

class Pt4D {
public:
        constexpr static std::size_t dim = 4;

        int x, y, z, w;

        template <std::size_t D>
        int get() const {
                static_assert(0 <= D && D <= 3, "Dimension should be in range");
                switch (D) {
                        case 0: return x;
                        case 1: return y;
                        case 2: return z;
                        case 3: return w;
                }
        }
};

class PtIntStr {
        constexpr static std::size_t dim = 2;
        int x;
        std::string y;

        template <std::size_t D, typename T>
        T get() const {
                static_assert(!std::is_same<T, T>::value, "This should not be instanciated");
        }
};

template <>
int PtIntStr::get<0>() const {
        return x;
}

template <>
std::string PtIntStr::get<1>() const {
        return y;
}


TEST(KdTreeTest, BuildHasLimitedHeight)
{
        std::vector<Pt2D> points;
        auto tree = KdTree<Pt2D>::Build(points);
        EXPECT_LE(tree.Height(), std::log2((double)points.size() + 1) + 1);
}

TEST(KdTreeTest, RangeQuery)
{
}

TEST(KdTreeTest, Insert)
{
}

TEST(KdTreeTest, HigherDimensional)
{
}

TEST(KdTreeTest, NonArithmeticDimension)
{
}

} // namespace

