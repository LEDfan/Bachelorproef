#include "HighSchoolPopulator.h"
#include "../HighSchool.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/HighSchool.h>
#include <iostream>
#include <pop/Person.h>

namespace gengeopop {

HighSchoolPopulator::HighSchoolPopulator(stride::util::RNManager& rn_manager) : PartialPopulator(rn_manager) {}

void HighSchoolPopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        std::cout << std::endl << "Starting to populate HighSchools" << std::endl;
        std::set<std::shared_ptr<ContactPool>> found;
        unsigned int                           students  = 0;
        unsigned int                           commuting = 0;

        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                // 1. find all highschools in an area of 10-k*10 km
                std::vector<std::shared_ptr<ContactPool>> nearByHighSchools =
                    GetContactPoolInIncreasingRadius<HighSchool>(geoGrid, loc);
                auto distNonCommuting = m_rnManager.GetGenerator(trng::uniform_int_dist(
                    0, static_cast<trng::uniform_int_dist::result_type>(nearByHighSchools.size())));

                // 2. find all highschools were students from this location commute to
                std::vector<std::shared_ptr<Location>> commutingHighSchools;
                std::vector<double>                    commutingWeights;
                for (const std::pair<std::shared_ptr<Location>, double>& commute : loc->getOutgoingCommuningCities()) {
                        auto highSchools = commute.first->getContactCentersOfType<HighSchool>();
                        if (!highSchools.empty()) {
                                commutingHighSchools.push_back(commute.first);
                                commutingWeights.push_back(commute.second);
                        }
                }

                std::function<trng::discrete_dist::result_type()> disCommuting;

                if (!commutingWeights.empty()) {
                        disCommuting = m_rnManager.GetGenerator(
                            trng::discrete_dist(commutingWeights.begin(), commutingWeights.end()));
                }

                // 2. for every student assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->getContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool> contactPool = household->GetPools()[0];
                        found.insert(contactPool);
                        for (const std::shared_ptr<stride::Person>& person : *contactPool) {
                                if (person->GetAge() >= 18 && person->GetAge() < 26 &&
                                    MakeChoice(geoGridConfig.input.fraction_1826_years_WhichAreStudents)) {
                                        students++;
                                        // this person is a student
                                        if (!commutingHighSchools.empty() &&
                                            MakeChoice(geoGridConfig.input.fraction_commutingPeople)) {
                                                // this person is commuting
                                                commuting++;

                                                // id of the location this person is commuting to
                                                auto locationId = disCommuting();
                                                // create a list of ContactPools (i.e. classes) for each of highschool
                                                // of this location
                                                auto highSchools = commutingHighSchools[locationId]
                                                                       ->getContactCentersOfType<HighSchool>();

                                                std::vector<std::shared_ptr<ContactPool>> contactPools;
                                                for (const auto& highSchool : highSchools) {
                                                        contactPools.insert(contactPools.end(), highSchool->begin(),
                                                                            highSchool->end());
                                                }

                                                auto disPools = m_rnManager.GetGenerator(trng::uniform_int_dist(
                                                    0, static_cast<trng::uniform_int_dist::result_type>(
                                                           contactPools.size())));

                                                auto id = disPools();
                                                contactPools[id]->addMember(person);
                                                person->SetHighSchoolId(static_cast<unsigned int>(id));
                                        } else {
                                                auto id = distNonCommuting();
                                                nearByHighSchools[id]->addMember(person);
                                                person->SetHighSchoolId(static_cast<unsigned int>(id));
                                        }
                                }
                        }
                }
        }
        std::cout << "Finished populating HighSchools" << std::endl;
        std::cout << "Used " << found.size() << " different classes" << std::endl;
        std::cout << "Placed " << students << " students in HighSchools, " << commuting
                  << " of whom are commuting to a different location" << std::endl;
}

} // namespace gengeopop