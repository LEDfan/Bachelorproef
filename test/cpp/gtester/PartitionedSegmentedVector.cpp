#include "util/PartitionedSegmentedVector.h"

#include <gtest/gtest.h>
#include <ostream>
#include <stdexcept>

using namespace stride::util;

TEST(PartitionedSegmentedVector, BasicLoopTest)
{
        PartitionedSegmentedVector<int> partitionedSegmentedVector(2);
        partitionedSegmentedVector.push_back(0, 0);
        partitionedSegmentedVector.push_back(1, 0);
        partitionedSegmentedVector.push_back(2, 0);
        partitionedSegmentedVector.push_back(3, 0);
        partitionedSegmentedVector.push_back(4, 1);
        partitionedSegmentedVector.push_back(5, 1);
        partitionedSegmentedVector.push_back(6, 1);
        partitionedSegmentedVector.push_back(7, 1);

        int i = 0;
        for (int x : partitionedSegmentedVector) {
                EXPECT_EQ(i, x);
                ++i;
        }

        EXPECT_TRUE(partitionedSegmentedVector.end() == partitionedSegmentedVector.end());
        EXPECT_TRUE(partitionedSegmentedVector.end()++ == partitionedSegmentedVector.end());

        auto it = partitionedSegmentedVector.begin();
        for (std::size_t i = 0; i < 10; i++) {
                it++;
        }
        EXPECT_EQ(partitionedSegmentedVector.end(), it);
}

TEST(PartitionedSegmentedVector, BasicReverseLoopTest)
{
        PartitionedSegmentedVector<int> partitionedSegmentedVector(2);
        partitionedSegmentedVector.push_back(0, 0);
        partitionedSegmentedVector.push_back(1, 0);
        partitionedSegmentedVector.push_back(2, 0);
        partitionedSegmentedVector.push_back(3, 0);
        partitionedSegmentedVector.push_back(4, 1);
        partitionedSegmentedVector.push_back(5, 1);
        partitionedSegmentedVector.push_back(6, 1);
        partitionedSegmentedVector.push_back(7, 1);

        int i = 7;
        for (auto it = --partitionedSegmentedVector.end(); it != partitionedSegmentedVector.begin(); --it) {
                EXPECT_EQ(i, *it);
                --i;
        }

        EXPECT_TRUE(partitionedSegmentedVector.end() == partitionedSegmentedVector.end());
        EXPECT_TRUE(partitionedSegmentedVector.end()++ == partitionedSegmentedVector.end());

        auto it = partitionedSegmentedVector.begin();
        for (std::size_t i = 0; i < 10; i++) {
                it++;
        }
        EXPECT_EQ(partitionedSegmentedVector.end(), it);
}

TEST(PartitionedSegmentedVector, RandomAccess)
{
        PartitionedSegmentedVector<int> partitionedSegmentedVector(6);
        partitionedSegmentedVector.push_back(0, 0);
        partitionedSegmentedVector.push_back(1, 0);
        partitionedSegmentedVector.push_back(2, 0);
        partitionedSegmentedVector.push_back(3, 0);
        partitionedSegmentedVector.push_back(4, 1);
        partitionedSegmentedVector.push_back(5, 2);
        partitionedSegmentedVector.push_back(6, 3);
        partitionedSegmentedVector.push_back(7, 4);
        partitionedSegmentedVector.push_back(8, 4);
        partitionedSegmentedVector.push_back(9, 4);
        partitionedSegmentedVector.push_back(10, 5);
        EXPECT_EQ(11, partitionedSegmentedVector.size());
        EXPECT_FALSE(partitionedSegmentedVector.IsFinalized());
        partitionedSegmentedVector.Finalize();
        EXPECT_TRUE(partitionedSegmentedVector.IsFinalized());
        EXPECT_EQ(11, partitionedSegmentedVector.size());

        for (std::size_t i = 0; i < partitionedSegmentedVector.size(); ++i) {
                EXPECT_EQ(i, partitionedSegmentedVector.at(i));
        }
}

struct storage
{
        int a;
};

TEST(PartitionedSegmentedVector, Finalizing)
{
        PartitionedSegmentedVector<storage> partitionedSegmentedVector(1);
        partitionedSegmentedVector.push_back(storage{0}, 0);
        partitionedSegmentedVector.Finalize();

        storage& firstElement = partitionedSegmentedVector.at(0);

        firstElement.a = 10;

        EXPECT_EQ(10, partitionedSegmentedVector.at(0).a);
}

TEST(PartitionedSegmentedVector, GetPartition)
{
        std::vector<int> exp{10, 24, 15, 0, 42};

        PartitionedSegmentedVector<int> partitionedSegmentedVector(2);

        PartitionedSegmentedVector<int>::segmentedVector_type& partition0 = partitionedSegmentedVector.GetPartition(0);
        PartitionedSegmentedVector<int>::segmentedVector_type& partition1 = partitionedSegmentedVector.GetPartition(1);

        partition1.emplace_back(0);
        partition0.emplace_back(10);
        partition1.emplace_back(42);
        partition0.emplace_back(24);
        partition0.emplace_back(15);

        partitionedSegmentedVector.Finalize();

        for (std::size_t i = 0; i < exp.size(); ++i) {
                EXPECT_EQ(exp[i], partitionedSegmentedVector.at(i));
        }

        EXPECT_THROW(partition1.emplace_back(100), Exception);
}

TEST(PartitionedSegmentedVector, ConstIterators)
{
        PartitionedSegmentedVector<int> partitionedSegmentedVector(2);

        PartitionedSegmentedVector<int>::segmentedVector_type& partition0 = partitionedSegmentedVector.GetPartition(0);
        PartitionedSegmentedVector<int>::segmentedVector_type& partition1 = partitionedSegmentedVector.GetPartition(1);

        partition0.emplace_back(10);
        partition0.emplace_back(24);
        partition0.emplace_back(15);
        partition1.emplace_back(0);
        partition1.emplace_back(42);

        partitionedSegmentedVector.Finalize();

        int i = 0;
        for (auto it = partitionedSegmentedVector.cbegin(); it != partitionedSegmentedVector.cend(); ++it) {
                EXPECT_EQ(partitionedSegmentedVector.at(i), *it);
                ++i;
        }

        i = 4;
        for (auto it = --partitionedSegmentedVector.cend(); it != partitionedSegmentedVector.cbegin(); --it) {
                EXPECT_EQ(partitionedSegmentedVector.at(i), *it);
                --i;
        }
}

template <typename Nesting, typename T>
class Nester : public std::vector<Nesting>
{
public:
        using iterator = NestedIterator<T, typename std::vector<Nesting>::iterator, typename Nesting::iterator>;

        iterator begin()
        {
                return iterator(std::vector<Nesting>::begin(), std::vector<Nesting>::begin(),
                                std::vector<Nesting>::end());
        }

        iterator end()
        {
                return iterator(std::vector<Nesting>::end(), std::vector<Nesting>::begin(),
                                std::vector<Nesting>::end());
        }
};

TEST(PartitionedSegmentedVector, TripleIterator)
{
        Nester<Nester<Nester<Nester<std::vector<int>, int>, int>, int>, int> tripleVector;
        tripleVector.emplace_back();
        tripleVector[0].emplace_back();
        tripleVector[0][0].emplace_back();
        tripleVector[0][0][0].emplace_back(std::vector<int>({0, 1, 2, 3}));
        tripleVector[0][0][0].emplace_back(std::vector<int>({4, 5, 6, 7}));
        tripleVector[0].emplace_back();
        tripleVector[0][1].emplace_back();
        tripleVector[0][1][0].emplace_back(std::vector<int>({8, 9, 10, 11}));
        tripleVector.emplace_back();
        tripleVector[1].emplace_back();
        tripleVector[1][0].emplace_back();
        tripleVector[1][0][0].emplace_back(std::vector<int>({12, 13, 14, 15}));

        int i = 0;
        for (int x : tripleVector) {
                EXPECT_EQ(i, x);
                ++i;
        }
        EXPECT_EQ(i, 16);

        i = 0;
        for (int x : tripleVector[0][0][0]) {
                EXPECT_EQ(i, x);
                ++i;
        }
        EXPECT_EQ(i, 8);
}
