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

#include "TravellerProfile.h"
#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <pop/Population.h>

namespace stride {

/**
 * Builds a TravellerProfile from the values in a configPt (using csv files).
 */
class TravellerProfileBuilder
{
public:
        /// Constructor
        TravellerProfileBuilder(const boost::property_tree::ptree& configPt, util::RnMan& rnManager,
                                std::shared_ptr<Population> pop);

        /// Build and return the TravellerProfile
        std::shared_ptr<TravellerProfile> Build();

private:
        /// Call the provided updateMethod with the traveller information found in the configParam and the given regions
        void AddTravellerInformation(std::string configParam, std::unordered_map<std::string, std::size_t> regions,
                                     std::function<void(std::size_t, std::size_t, double)> updateMethod);

        const boost::property_tree::ptree& m_configPt;  ///< Configuration sued
        util::RnMan&                       m_rnManager; ///< RnManager (passed to created TravellerProfile)
        std::shared_ptr<Population>        m_pop;       ///< Population
};

} // namespace stride
