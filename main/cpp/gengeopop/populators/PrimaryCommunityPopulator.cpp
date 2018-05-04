#include "PrimaryCommunityPopulator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/PrimaryCommunity.h>
#include <iostream>
#include <pop/Person.h>

namespace gengeopop {

void PrimaryCommunityPopulator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig&)
{

        m_logger->info("Starting to populate Primary Communities");

        std::set<std::shared_ptr<ContactPool>> found;
        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                if (loc->GetPopulation() == 0) {
                        continue;
                }
                // 1. find all communities in an area of 10-k*10 km
                const auto& community_pools = GetContactPoolInIncreasingRadius<PrimaryCommunity>(geoGrid, loc);

                // 2. for every household assign a community
                const auto dist = m_rnManager.GetGenerator(trng::uniform_int_dist(
                    0, static_cast<trng::uniform_int_dist::result_type>(community_pools.size())));

                for (const std::shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool>& contactPool = household->GetPools()[0];
                        const auto                          pool        = dist();
                        for (stride::Person* person : *contactPool) {
                                found.insert(community_pools[pool]);
                                if (community_pools[pool].get() == nullptr) {
                                        break;
                                }
                                community_pools[pool]->AddMember(person);
                        }
                }
        }
        m_logger->info("Finished populating Primary Communities");
        m_logger->info("Used {} different Primary communities", found.size());
}

} // namespace gengeopop
