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

PrimaryCommunityPopulator::PrimaryCommunityPopulator(stride::util::RNManager& rn_manager) : PartialPopulator(rn_manager)
{
}

void PrimaryCommunityPopulator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig &)
{

        // for every location
        std::cout << std::endl << "Starting to populate Primary Communities" << std::endl;
        unsigned int                           full_capacity_count = 0;
        std::set<std::shared_ptr<ContactPool>> found;
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                std::vector<std::shared_ptr<ContactPool>> community_pools;
                std::function<unsigned int()>             dist;

                auto updatePools = [this, &geoGrid, &loc, &community_pools, &dist]() {
                        // 1. find all communities in an area of 10-k*10 km
                        if (community_pools.empty())
                                community_pools = GetContactPoolInIncreasingRadius<PrimaryCommunity>(geoGrid, loc);

                        // 2. for every household assign a community
                        dist = m_rnManager.GetGenerator(trng::uniform_int_dist(
                            0, static_cast<trng::uniform_int_dist::result_type>(community_pools.size())));
                };

                updatePools();
                for (const std::shared_ptr<ContactCenter>& household : loc->GetContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool> contactPool = household->GetPools()[0];
                        auto                               pool        = dist();
                        for (const std::shared_ptr<stride::Person>& person : *contactPool) {
                                found.insert(community_pools[pool]);
                                if (community_pools[pool].get() == nullptr) {
                                        std::cout << pool << std::endl;
                                        break;
                                }
                                community_pools[pool]->AddMember(person);
                        }
                        if (community_pools[pool]->GetCapacity() <= community_pools[pool]->GetUsedCapacity()) {
                                full_capacity_count++;
                                community_pools.erase(community_pools.begin() + pool);
                                updatePools();
                                if (community_pools.empty()) {
                                        std::cout << "Could not find a pool for person" << std::endl;
                                        return;
                                }
                        }
                }
        }
        std::cout << "Finished populating Primary Communities" << std::endl;
        std::cout << "Used " << found.size() << " different Primary communities" << std::endl;
        std::cout << "Filled " << full_capacity_count << " Primary communities completely" << std::endl;
}

} // namespace gengeopop
