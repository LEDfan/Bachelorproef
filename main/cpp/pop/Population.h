#pragma once
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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Header file for the core Population class
 */

#include "pop/Person.h"
#include "util/SegmentedVector.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cassert>
#include <memory>
#include <vector>

namespace stride {

/**
 * Container for persons in population.
 */
class Population : public std::vector<Person>
{
public:
        Population() : beliefs_container() {};

        ///
        unsigned int GetAdoptedCount() const;

        /// Get the cumulative number of cases.
        unsigned int GetInfectedCount() const;

        /// New Person in the population.
        void CreatePerson(unsigned int id, double age, unsigned int household_id, unsigned int school_id,
                          unsigned int work_id, unsigned int primary_community_id, unsigned int secondary_community_id,
                          unsigned int start_infectiousness, unsigned int start_symptomatic,
                          unsigned int time_infectious, unsigned int time_symptomatic,
                          const boost::property_tree::ptree& pt_belief, double risk_averseness = 0);

private:
        ///
        template <typename BeliefPolicy>
        void NewPerson(unsigned int id, double age, unsigned int household_id, unsigned int school_id,
                       unsigned int work_id, unsigned int primary_community_id, unsigned int secondary_community_id,
                       unsigned int start_infectiousness, unsigned int start_symptomatic, unsigned int time_infectious,
                       unsigned int time_symptomatic, const boost::property_tree::ptree& pt_belief,
                       double risk_averseness = 0);

        util::SegmentedVector<std::unique_ptr<Belief>> beliefs_container;
};

} // namespace stride
