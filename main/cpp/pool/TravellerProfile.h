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

#include "ContactPool.h"
#include <trng/discrete_dist.hpp>
#include <util/RnMan.h>
#include <vector>

namespace stride {

class Population;

/**
 * Holds the information about travels
 */
class TravellerProfile
{
public:
        /// AmountOfTravel is a fraction 0-1 which indicates the change for a person to travel
        /// fractionWork indicates the amount of work travel
        TravellerProfile(std::size_t amountOfRegions, double amountOfTravel, double fractionWork,
                         util::RnMan& rnManager, std::size_t maxDays);

        explicit TravellerProfile(util::RnMan& rnManager)
            : m_data_recreation(), m_data_work(), m_amountOfTravel(), m_fractionWork(), m_rnManager(rnManager),
              m_maxDays(){};

        /// Add a travel record to the recreation profile
        void AddTravelRecreation(std::size_t from, std::size_t to, double relativePopulation);

        /// Add a travel record to the work profile
        void AddTravelWork(std::size_t from, std::size_t to, double relativePopulation);

        /// Based on the profile determines if a person will travel and handle when it effectively travels
        bool PersonWillTravel(size_t currentRegion, std::shared_ptr<Population> ptr, Person* person,
                              std::size_t currentDay);

private:
        /// Holds the actual travelling profile used to "generate" trips
        /// From region, to region, relative amount of people
        std::vector<std::vector<double>> m_data_recreation;

        /// Holds the actual travelling profile used to "generate" trips
        /// From region, to region, relative amount of people
        std::vector<std::vector<double>> m_data_work;

        /// A fraction 0-1 which indicates the change for a person to travel
        double m_amountOfTravel;

        /// A fraction 0-1 which indicates whether it's work travel or recreation
        double m_fractionWork;

        util::RnMan& m_rnManager;

        /// Maximum amount of days to travel
        std::size_t m_maxDays;

        /// Helper function to make a random choice based on a fraction
        bool MakeChoice(double fraction);
};

} // namespace stride
