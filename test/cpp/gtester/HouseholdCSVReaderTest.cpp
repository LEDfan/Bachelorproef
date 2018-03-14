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

#include <gengeopop/io/HouseholdCSVReader.h>
#include <gtest/gtest.h>
#include <memory>

namespace {

TEST(HouseholdCSVReader, test1)
{
        std::string csvString =
            "hh_age1,hh_age2,hh_age3,hh_age4,hh_age5,hh_age6,hh_age7,hh_age8,hh_age9,hh_age10,hh_age11,hh_age12\n"
            "42,38,15,NA,NA,NA,NA,NA,NA,NA,NA,NA\n";
        std::istringstream                                 instream(csvString);
        gengeopop::HouseholdCSVReader                      reader(instream);
        std::vector<std::shared_ptr<gengeopop::Household>> HHs = reader.getHouseHolds();

        ASSERT_EQ(HHs.size(), (unsigned int)1);
        // TODO test more
}

} // namespace
