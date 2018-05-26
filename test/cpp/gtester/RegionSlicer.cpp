#include <gtest/gtest.h>
#include <pop/Population.h>
#include <pop/RegionSlicer.h>

TEST(RegionSlicer, Basic)
{
        //        std::shared_ptr<stride::Population> pop = stride::Population::Create();
        //        pop->CreatePerson(0, 0, 10, 1, 1, 1, 1, 1);
        //        pop->CreatePerson(0, 1, 11, 2, 2, 2, 2, 2);
        //        pop->CreatePerson(0, 1, 12, 1, 2, 1, 2, 1);

        //        ASSERT_EQ((*(pop->begin())).GetAge(), 10);
        //
        //        pop->Finalize();
        //        RegionSlicer slicer(pop);
        //        slicer[stride::ContactPoolType::Id::Household];
        //        auto it = slicer.begin();
        //        // First Household
        //        ASSERT_EQ(10, (*it++)->GetAge());
        //        ASSERT_EQ(12, (*it++)->GetAge());
        //        // Second Household
        //        ASSERT_EQ(11, (*it++)->GetAge());
        //        ASSERT_EQ(it, slicer.end());
}

TEST(RegionSlicer, MultiRegionContactpoolType)
{
        //        std::shared_ptr<stride::Population> pop = stride::Population::Create(2);
        //        pop->CreatePerson(0, 0, 10, 1, 1, 1, 1, 1);
        //        pop->CreatePerson(0, 1, 11, 2, 2, 2, 2, 2);
        //        pop->CreatePerson(1, 1, 12, 1, 2, 1, 2, 1);
        //
        //        ASSERT_EQ((*(pop->begin())).GetAge(), 10);
        //
        //        pop->Finalize();
        //        RegionSlicer slicer(pop);
        //        slicer[stride::ContactPoolType::Id::Household];
        //        auto it = slicer.begin();
        //        // First Household
        //        ASSERT_EQ(10, (*it++)->GetAge());
        //        ASSERT_EQ(11, (*it++)->GetAge());
        //        // Second Household
        //        ASSERT_EQ(12, (*it++)->GetAge());
        //        ASSERT_EQ(it, slicer.end());
}
