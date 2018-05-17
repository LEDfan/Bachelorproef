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

        for (int x : partitionedSegmentedVector) {
                std::cout << x << std::endl;
        }
}
