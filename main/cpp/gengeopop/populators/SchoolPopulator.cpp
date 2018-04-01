#include "SchoolPopulator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/School.h>
#include <iostream>
#include <pop/Person.h>

namespace gengeopop {

SchoolPopulator::SchoolPopulator(stride::util::RNManager& rn_manager) : PartialPopulator(rn_manager) {}

void SchoolPopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{

        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                // 1. find all schools in an area of 10-k*10 km
                std::vector<std::shared_ptr<ContactPool>> classes =
                    GetContactPoolInIncreasingRadius<School>(geoGrid, loc);

                auto dist = m_rnManager.GetGenerator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(classes.size())));

                // 2. for every student assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->getContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool> contactPool = household->GetPools()[0];
                        for (const std::shared_ptr<stride::Person>& person : *contactPool) {
                                if (person->GetAge() < 18 && person->GetAge() >= 6) {
                                        auto id = dist();
                                        classes[id]->addMember(person);
                                        person->SetSchoolId(static_cast<unsigned int>(id));
                                }
                        }
                }
        }
}

} // namespace gengeopop
