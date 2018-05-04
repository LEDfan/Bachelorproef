#include "SchoolPopulator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/School.h>
#include <iostream>
#include <pop/Person.h>
#include <set>

namespace gengeopop {

void SchoolPopulator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig&)
{
        m_logger->info("Starting to populate Schools");

        std::set<std::shared_ptr<ContactPool>> found;
        unsigned int                           pupils = 0;

        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                if (loc->GetPopulation() == 0) {
                        continue;
                }

                // 1. find all schools in an area of 10-k*10 km
                const std::vector<std::shared_ptr<ContactPool>>& classes =
                    GetContactPoolInIncreasingRadius<School>(geoGrid, loc);

                auto dist = m_rnManager.GetGenerator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(classes.size())));

                // 2. for every student assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool>& contactPool = household->GetPools()[0];
                        found.insert(contactPool);
                        for (stride::Person* person : *contactPool) {
                                if (person->GetAge() < 18 && person->GetAge() >= 6) {
                                        auto id = dist();
                                        classes[id]->AddMember(person);
                                        person->SetSchoolId(static_cast<unsigned int>(id));
                                        pupils++;
                                }
                        }
                }
        }

        m_logger->info("Finished populating Schools");
        m_logger->info("Used {} different classes", found.size());
        m_logger->info("Placed {} pupils in Schools", pupils);
}

} // namespace gengeopop
