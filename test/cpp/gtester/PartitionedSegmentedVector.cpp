#include "util/PartitionedSegmentedVector.h"

#include <gtest/gtest.h>
#include <ostream>
#include <stdexcept>

using namespace stride::util;

TEST(PartitionedSegmentedVector, BasicLoopTest)
{

        PartitionedSegmentedVector<int> partitionedSegmentedVector(2);
        partitionedSegmentedVector.push_back(0, 0);
        partitionedSegmentedVector.push_back(0, 1);
        partitionedSegmentedVector.push_back(0, 2);
        partitionedSegmentedVector.push_back(0, 3);
        partitionedSegmentedVector.push_back(1, 4);
        partitionedSegmentedVector.push_back(1, 5);
        partitionedSegmentedVector.push_back(1, 6);
        partitionedSegmentedVector.push_back(1, 7);

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
        partitionedSegmentedVector.push_back(0, 1);
        partitionedSegmentedVector.push_back(0, 2);
        partitionedSegmentedVector.push_back(0, 3);
        partitionedSegmentedVector.push_back(1, 4);
        partitionedSegmentedVector.push_back(1, 5);
        partitionedSegmentedVector.push_back(1, 6);
        partitionedSegmentedVector.push_back(1, 7);

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
        partitionedSegmentedVector.push_back(0, 1);
        partitionedSegmentedVector.push_back(0, 2);
        partitionedSegmentedVector.push_back(0, 3);
        partitionedSegmentedVector.push_back(1, 4);
        partitionedSegmentedVector.push_back(2, 5);
        partitionedSegmentedVector.push_back(3, 6);
        partitionedSegmentedVector.push_back(4, 7);
        partitionedSegmentedVector.push_back(4, 8);
        partitionedSegmentedVector.push_back(4, 9);
        partitionedSegmentedVector.push_back(5, 10);
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
        partitionedSegmentedVector.push_back(0, storage{0});
        partitionedSegmentedVector.Finalize();

        storage& firstElement = partitionedSegmentedVector.at(0);

        firstElement.a = 10;

        EXPECT_EQ(10, partitionedSegmentedVector.at(0).a);
}

TEST(PartitionedSegmentedVector, GetPartition)
{

        std::vector<int> exp{10, 24, 15, 0, 42};

        PartitionedSegmentedVector<int> partitionedSegmentedVector(2);
        //        partitionedSegmentedVector.push_back(0, 0);
        //        partitionedSegmentedVector.push_back(1, 1);
        //        partitionedSegmentedVector.Finalize();

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

        //        EXPECT_TRUE(std::equal(partitionedSegmentedVector.begin(), partitionedSegmentedVector.end(),
        //        exp.begin(), exp.end()));
}
