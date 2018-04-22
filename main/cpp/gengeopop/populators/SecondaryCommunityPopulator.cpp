#include "SecondaryCommunityPopulator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/SecondaryCommunity.h>
#include <iostream>
#include <pop/Person.h>

namespace gengeopop {

void SecondaryCommunityPopulator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig&)
{

        m_logger->info("Starting to populate Secondary Communities");

        unsigned int                           full_capacity_count = 0;
        std::set<std::shared_ptr<ContactPool>> found;
        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                // 1. find all communities in an area of 10-k*10 km
                std::vector<std::shared_ptr<ContactPool>> community_pools;
                std::function<unsigned int()>             dist;

                auto updatePools = [this, &geoGrid, &loc, &community_pools, &dist]() {
                        // 1. find all communities in an area of 10-k*10 km
                        if (community_pools.empty())
                                community_pools = GetContactPoolInIncreasingRadius<SecondaryCommunity>(geoGrid, loc);

                        // 2. for every household assign a community
                        dist = m_rnManager.GetGenerator(trng::uniform_int_dist(
                            0, static_cast<trng::uniform_int_dist::result_type>(community_pools.size())));
                };

                updatePools();

                for (const std::shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool> contactPool = household->GetPools()[0];
                        for (stride::Person* person : *contactPool) {
                                auto pool = dist();
                                found.insert(community_pools[pool]);
                                community_pools[pool]->AddMember(person);
                                if (community_pools[pool]->GetCapacity() <= community_pools[pool]->GetUsedCapacity()) {
                                        full_capacity_count++;
                                        community_pools.erase(community_pools.begin() + pool);
                                        updatePools();
                                        if (community_pools.empty()) {
                                                m_logger->warn("Could not find a pool for person");
                                                break;
                                        }
                                }
                        }
                        if (community_pools.empty()) {
                                break;
                        }
                }
        }

        m_logger->info("Finished populating Secondary Communities");
        m_logger->info("Used {} different Secondary communities", found.size());
        m_logger->info("Filled {} Secondary communities completely", full_capacity_count);
}

} // namespace gengeopop
