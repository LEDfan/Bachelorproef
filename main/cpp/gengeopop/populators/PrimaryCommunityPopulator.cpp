#include "PrimaryCommunityPopulator.h"
#include "gengeopop/K12School.h"
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

        std::set<stride::ContactPool*> found;
        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                if (loc->GetPopulation() == 0) {
                        continue;
                }

                // 1. find all communities in an area of 10-k*10 km
                const auto& community_pools = GetContactPoolInIncreasingRadius<PrimaryCommunity>(geoGrid, loc);

                // 2. for every household assign a community
                const auto dist = m_rnManager[0].variate_generator(trng::uniform_int_dist(
                    0, static_cast<trng::uniform_int_dist::result_type>(community_pools.size())));

                for (const std::shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                        stride::ContactPool* contactPool = household->GetPools()[0];
                        for (stride::Person* person : *contactPool) {
                                const auto pool = dist();
                                found.insert(community_pools[pool]);
                                community_pools[pool]->AddMember(person);
                                person->SetPrimaryCommunityId(community_pools[pool]->GetId());
                        }
                }
        }

        m_logger->info("Finished populating Primary Communities");
        m_logger->info("Used {} different Primary communities", found.size());
}

} // namespace gengeopop
