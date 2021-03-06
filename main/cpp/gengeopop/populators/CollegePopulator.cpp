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

#include "CollegePopulator.h"
#include "gengeopop/College.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/College.h>
#include <gengeopop/GeoGridConfig.h>
#include <iostream>
#include <pop/Person.h>
#include <util/ExcAssert.h>

namespace gengeopop {

void CollegePopulator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        m_logger->info("Starting to populate Colleges");

        std::set<stride::ContactPool*> found;
        unsigned int                   students  = 0;
        unsigned int                   commuting = 0;

        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                if (loc->GetPopulation() == 0) {
                        continue;
                }
                // 1. find all highschools in an area of 10-k*10 km
                const std::vector<stride::ContactPool*>& nearByHighSchools =
                    GetContactPoolInIncreasingRadius<College>(geoGrid, loc);

                ExcAssert(!nearByHighSchools.empty(),
                          "Did not find any HighSchool due to invalid input data in HighSchoolPopulator");

                auto distNonCommuting = m_rnManager[0].variate_generator(trng::uniform_int_dist(
                    0, static_cast<trng::uniform_int_dist::result_type>(nearByHighSchools.size())));

                // 2. find all highschools were students from this location commute to
                std::vector<Location*> commutingHighSchools;
                std::vector<double>    commutingWeights;
                for (const std::pair<Location*, double>& commute : loc->GetOutgoingCommuningCities()) {
                        const auto& highSchools = commute.first->GetContactCentersOfType<College>();
                        if (!highSchools.empty()) {
                                commutingHighSchools.push_back(commute.first);
                                commutingWeights.push_back(commute.second);
                        }
                }

                std::function<trng::discrete_dist::result_type()> disCommuting;

                if (!commutingWeights.empty()) {
                        disCommuting = m_rnManager[0].variate_generator(
                            trng::discrete_dist(commutingWeights.begin(), commutingWeights.end()));
                }

                // 2. for every student assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                        stride::ContactPool* contactPool = household->GetPools()[0];
                        found.insert(contactPool);
                        for (stride::Person* person : *contactPool) {
                                if (person->IsCollegeStudentCandidate() &&
                                    MakeChoice(geoGridConfig.input.fraction_1826_years_WhichAreStudents)) {
                                        students++;
                                        // this person is a student
                                        if (!commutingHighSchools.empty() &&
                                            MakeChoice(geoGridConfig.input.fraction_student_commutingPeople)) {
                                                // this person is commuting
                                                commuting++;

                                                // id of the location this person is commuting to
                                                auto locationId = disCommuting();
                                                // create a list of ContactPools (i.e. classes) for each of highschool
                                                // of this location
                                                const auto& highSchools = commutingHighSchools[locationId]
                                                                              ->GetContactCentersOfType<College>();

                                                std::vector<stride::ContactPool*> contactPools;
                                                for (const auto& highSchool : highSchools) {
                                                        contactPools.insert(contactPools.end(), highSchool->begin(),
                                                                            highSchool->end());
                                                }

                                                auto disPools = m_rnManager[0].variate_generator(trng::uniform_int_dist(
                                                    0, static_cast<trng::uniform_int_dist::result_type>(
                                                           contactPools.size())));

                                                auto id = disPools();
                                                contactPools[id]->AddMember(person);
                                                person->SetCollegeId(contactPools[id]->GetId());
                                        } else {
                                                auto id = distNonCommuting();
                                                nearByHighSchools[id]->AddMember(person);
                                                person->SetCollegeId(nearByHighSchools[id]->GetId());
                                        }
                                }
                        }
                }
        }
        m_logger->info("Finished populating Colleges");
        m_logger->info("Used {} different classes", found.size());
        m_logger->info("Placed {} students in Colleges, {} of whom are commuting to a different location", students,
                       commuting);
}

} // namespace gengeopop
