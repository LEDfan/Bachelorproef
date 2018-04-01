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

SecondaryCommunityPopulator::SecondaryCommunityPopulator(stride::util::RNManager& rn_manager)
    : PartialPopulator(rn_manager)
{
}

void SecondaryCommunityPopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig&)
{

        // for every location
        std::cout << std::endl << "Starting to populate Secondary Communities" << std::endl;
        unsigned int                           full_capacity_count = 0;
        std::set<std::shared_ptr<ContactPool>> found;
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

                for (const std::shared_ptr<ContactCenter>& household : loc->getContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool> contactPool = household->GetPools()[0];
                        for (const std::shared_ptr<stride::Person>& person : *contactPool) {
                                auto pool = dist();
                                found.insert(community_pools[pool]);
                                community_pools[pool]->addMember(person);
                                if (community_pools[pool]->getCapacity() <= community_pools[pool]->getUsedCapacity()) {
                                        full_capacity_count++;
                                        community_pools.erase(community_pools.begin() + pool);
                                        updatePools();
                                        if (community_pools.empty()) {
                                                std::cout << "Could not find a pool for person" << std::endl;
                                                break;
                                        }
                                }
                        }
                        if (community_pools.empty()) {
                                break;
                        }
                }
        }
        std::cout << "Finished populating Secondary Communities" << std::endl;
        std::cout << "Used " << found.size() << " different Secondary communities" << std::endl;
        std::cout << "Filled " << full_capacity_count << " Secondary communities completely" << std::endl;
}

} // namespace gengeopop
