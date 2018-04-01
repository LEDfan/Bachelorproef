#include "HighSchoolPopulator.h"
#include "../HighSchool.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/HighSchool.h>
#include <iostream>
#include <pop/Person.h>
#include <gengeopop/GeoGridConfig.h>

namespace gengeopop {

HighSchoolPopulator::HighSchoolPopulator(stride::util::RNManager& rn_manager) : PartialPopulator(rn_manager) {}

void HighSchoolPopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig) {
        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                // 1. find all highschools in an area of 10-k*10 km
                std::vector<std::shared_ptr<ContactPool>> nearByHighSchools = GetContactPoolInIncreasingRadius<HighSchool>(geoGrid, loc);
                auto distNonCommuting = m_rnManager.GetGenerator(
                        trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(nearByHighSchools.size())));

                // 2. find all highschools were students from this location commute to
                std::vector<std::shared_ptr<ContactPool>> commutingHighSchools;
                for (const std::pair<std::shared_ptr<Location>, double> & commute : loc->getOutgoingCommuningCities()) {
                        auto contactCenters = commute.first->getContactCentersOfType<HighSchool>();
                        for (const auto& contactCenter : contactCenters) {
                                commutingHighSchools.insert(commutingHighSchools.end(), contactCenter->begin(), contactCenter->end());
                        }
                }

                auto disCommuting = m_rnManager.GetGenerator(
                        trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(commutingHighSchools.size())));

                // 2. for every student assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->getContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool> contactPool = household->GetPools()[0];
                        for (const std::shared_ptr<stride::Person>& person : *contactPool) {
                                if (person->GetAge() >= 18 && person->GetAge() < 26 && MakeChoice(geoGridConfig.input.fraction_1826_years_WhichAreStudents)) {
                                        // this person is a student
                                        if (!commutingHighSchools.empty() && MakeChoice(geoGridConfig.input.fraction_commutingPeople)) {
                                                // this person is commuting
                                                commutingHighSchools[disCommuting()]->addMember(person);
                                        } else {
                                                nearByHighSchools[distNonCommuting()]->addMember(person);
                                        }
                                }
                        }
                }
        }

}

} // namespace gengeopop
