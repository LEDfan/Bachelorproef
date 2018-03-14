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

#include <gengeopop/io/CitiesCSVReader.h>
#include <gtest/gtest.h>

namespace {

TEST(CitiesCSVReaderTest, test1)
{
        std::string csvString = "id,province,population,x_coord,y_coord,latitude,longitude,name\n"
                                "11001,1,8249,150964.3953,202431.664,51.1340539,4.3844742,AARTSELAAR";
        std::istringstream                                instream(csvString);
        gengeopop::CitiesCSVReader                        reader(instream);
        std::vector<std::shared_ptr<gengeopop::Location>> locs = reader.getLocations();

        ASSERT_EQ(locs.size(), (unsigned int)1);
        ASSERT_EQ(locs[0]->getName(), std::string("AARTSELAAR"));
        ASSERT_EQ(locs[0]->getProvince(), 1);
}

} // namespace
