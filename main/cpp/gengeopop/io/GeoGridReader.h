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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#pragma once

#include "../GeoGrid.h"
#include <memory>
#include <ostream>

namespace gengeopop {

/**
 * An abstract base class for creating a GeoGrid that was read from a file, can be implemented using multiple file types
 * (proto and json are currently implemented)
 */
class GeoGridReader
{
public:
        /// Construct the GeoGridReader with the istream containing the contents of the file and the Population used to
        /// create the Persons
        GeoGridReader(std::unique_ptr<std::istream> inputStream, stride::Population* pop, std::size_t regionId = 0,
                      std::string regionName = "Default");

        /// No copy constructor
        GeoGridReader(const GeoGridReader&) = delete;

        /// No copy assignment
        GeoGridReader& operator=(const GeoGridReader&) = delete;

        /// Default destructor
        virtual ~GeoGridReader() = default;

        /// Perform the actual read and return the created GeoGrid
        virtual std::shared_ptr<GeoGrid> Read() = 0;

protected:
        /// Add the commutes that were found to their respective Locations symmetrically
        void AddCommutes(std::shared_ptr<GeoGrid> geoGrid);

        std::map<unsigned int, stride::Person*>
                                                                    m_people; ///< Store the persons (id->person) that were found while loping over the ContactPools
        std::vector<std::tuple<unsigned int, unsigned int, double>> m_commutes; ///< from, to, amount
        std::size_t                                                 m_regionId; ///< RegionId, used to create persons
        std::string m_regionName; ///< RegionName, region name passed to GeoGrid

        std::unique_ptr<std::istream> m_inputStream; ///< file to read

        stride::Population* m_population; ///< population to use in the GeoGrid may be nullptr
};

} // namespace gengeopop
