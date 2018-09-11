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

#include "WorkplacePopulator.h"
#include "../../pool/ContactPool.h"
#include "../Workplace.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/Workplace.h>
#include <iostream>
#include <pop/Person.h>
#include <util/ExcAssert.h>
#include <utility>

namespace gengeopop {

WorkplacePopulator::WorkplacePopulator(stride::util::RnMan& rn_manager, std::shared_ptr<spdlog::logger> logger)
    : PartialPopulator(rn_manager, std::move(logger)), m_currentLoc(nullptr), m_geoGrid(nullptr), m_geoGridConfig(),
      m_workplacesInCity(), m_fractionCommutingStudents(0), m_nearByWorkplaces(), m_distNonCommuting(),
      m_commutingLocations(), m_disCommuting()
{
}

void WorkplacePopulator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        m_logger->info("Starting to populate Workplaces");

        m_geoGrid                   = geoGrid;
        m_geoGridConfig             = geoGridConfig;
        m_fractionCommutingStudents = 0;
        m_workplacesInCity.clear();
        m_currentLoc           = nullptr;
        m_assignedTo0          = 0;
        m_assignedCommuting    = 0;
        m_assignedNotCommuting = 0;
        m_distNonCommuting     = discreteDist();
        m_nearByWorkplaces.clear();
        m_disCommuting = discreteDist();
        m_commutingLocations.clear();

        CalculateFractionCommutingStudents();
        CalculateWorkplacesInCity();

        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                if (loc->GetPopulation() == 0) {
                        continue;
                }
                m_currentLoc = loc;
                CalculateCommutingLocations();
                CalculateNearbyWorkspaces();

                // 2. for every worker assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                        stride::ContactPool* contactPool = household->GetPools()[0];
                        for (stride::Person* person : *contactPool) {
                                if (person->IsWorkableCandidate()) {
                                        bool isStudent =
                                            MakeChoice(geoGridConfig.input.fraction_1826_years_WhichAreStudents);
                                        bool isActiveWorker =
                                            MakeChoice(geoGridConfig.input.fraction_1865_years_active);

                                        if ((person->IsCollegeStudentCandidate() && !isStudent) || isActiveWorker) {
                                                AssignActive(person);
                                        } else {
                                                // this person isn't an active employee
                                                person->SetWorkId(0);
                                                m_assignedTo0++;
                                        }
                                }
                        }
                }
        }

        m_logger->info("Populated workplaces, assigned to 0 {}, assigned (commuting) {} assigned (not commuting) {} ",
                       m_assignedTo0, m_assignedCommuting, m_assignedNotCommuting);
}

void WorkplacePopulator::CalculateFractionCommutingStudents()
{
        if (m_geoGridConfig.input.fraction_active_commutingPeople &&
            m_geoGridConfig.calculated._1865_and_years_active) {
                m_fractionCommutingStudents = (m_geoGridConfig.calculated._1826_years_and_student *
                                               m_geoGridConfig.input.fraction_student_commutingPeople) /
                                              (m_geoGridConfig.calculated._1865_and_years_active *
                                               m_geoGridConfig.input.fraction_active_commutingPeople);
        } else {
                m_fractionCommutingStudents = 0;
        }
}

void WorkplacePopulator::CalculateWorkplacesInCity()
{
        for (const std::shared_ptr<Location>& loc : *m_geoGrid) {
                const auto&                       Workplaces = loc->GetContactCentersOfType<Workplace>();
                std::vector<stride::ContactPool*> contactPools;

                for (const auto& Workplace : Workplaces) {
                        contactPools.insert(contactPools.end(), Workplace->begin(), Workplace->end());
                }

                auto disPools = m_rnManager[0].variate_generator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(contactPools.size())));

                m_workplacesInCity[loc] = {contactPools, disPools};
        }
}

void WorkplacePopulator::AssignActive(stride::Person* person)
{
        // this person is (student and active) or active
        if (!m_commutingLocations.empty() && MakeChoice(m_geoGridConfig.input.fraction_active_commutingPeople)) {
                // this person is commuting

                // id of the location this person is commuting to
                auto        locationId = m_disCommuting();
                const auto& info       = m_workplacesInCity[m_commutingLocations[locationId]];
                auto        id         = info.second();

                info.first[id]->AddMember(person);
                person->SetWorkId(info.first[id]->GetId());
                m_assignedCommuting++;
        } else {
                auto id = m_distNonCommuting();
                m_nearByWorkplaces[id]->AddMember(person);
                person->SetWorkId(m_nearByWorkplaces[id]->GetId());
                m_assignedNotCommuting++;
        }
}

void WorkplacePopulator::CalculateCommutingLocations()
{
        // find all Workplaces were employees from this location commute to
        m_commutingLocations.clear();
        m_disCommuting = discreteDist();

        std::vector<double> commutingWeights;
        for (const std::pair<std::shared_ptr<Location>, double>& commute : m_currentLoc->GetOutgoingCommuningCities()) {
                const auto& Workplaces = commute.first->GetContactCentersOfType<Workplace>();
                if (!Workplaces.empty()) {
                        m_commutingLocations.push_back(commute.first);
                        double weight = commute.second - (commute.second * m_fractionCommutingStudents);
                        commutingWeights.push_back(weight);
                        ExcAssert(weight >= 0 && weight <= 1 && !std::isnan(weight),
                                  "Invalid weight due to invalid input data in WorkplacePopulator, weight: " +
                                      std::to_string(weight));
                }
        }

        if (!commutingWeights.empty()) {
                m_disCommuting = m_rnManager[0].variate_generator(
                    trng::discrete_dist(commutingWeights.begin(), commutingWeights.end()));
        }
}

void WorkplacePopulator::CalculateNearbyWorkspaces()
{
        m_nearByWorkplaces = GetContactPoolInIncreasingRadius<Workplace>(m_geoGrid, m_currentLoc);
        m_distNonCommuting = m_rnManager[0].variate_generator(
            trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(m_nearByWorkplaces.size())));
}

} // namespace gengeopop
