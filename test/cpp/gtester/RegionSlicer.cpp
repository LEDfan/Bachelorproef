/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, 2018 Willem L, Kuylen E, Stijven S & Broeckhove J
 */

#include <gtest/gtest.h>
#include <pop/Population.h>
#include <pop/RegioSlicer.h>

/**
 * This is a basic test file. It has a simple usage of the most frequent needed features.
 * Can be copied and adapted to create new tests quickly.
 */
namespace {

class AFixture : public testing::Test
{
protected:
        virtual void SetUp()
        {
                // Set up test environ for every case
                i = 3;
        }

        virtual void TearDown()
        {
                // Clean up after test case
        }

        int i;
};

TEST(RegionSlicer, Basic)
{
        std::shared_ptr<stride::Population> pop = stride::Population::Create();
        pop->CreatePerson(0, 0, 10, 1, 1, 1, 1, 1);
        pop->CreatePerson(0, 1, 11, 2, 2, 2, 2, 2);

        ASSERT_EQ((*(pop->begin())).GetAge(), 10);

        pop->Finalize();
        RegioSlicer slicer(pop);
        slicer[stride::ContactPoolType::Id::Household];
        int i = 10;
        for (auto it = slicer.begin(); it != slicer.end(); it++) {
                ASSERT_EQ(i, (*it)->GetAge());
                i++;
        }
}

} // namespace
