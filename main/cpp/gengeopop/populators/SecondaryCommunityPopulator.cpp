#include "SecondaryCommunityPopulator.h"
#include "gengeopop/K12School.h"
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

        std::set<stride::ContactPool*> found;
        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                if (loc->GetPopulation() == 0) {
                        continue;
                }
                // 1. find all communities in an area of 10-k*10 km
                const auto& community_pools = GetContactPoolInIncreasingRadius<SecondaryCommunity>(geoGrid, loc);

                // 2. find all households in this location
                const auto& households = loc->GetContactCentersOfType<Household>();

                unsigned int households_per_community        = households.size() / community_pools.size();
                unsigned int remainder                       = households.size() % community_pools.size();
                unsigned int current_community               = 0;
                unsigned int current_households_in_community = 0;
                for (unsigned int i = 0; i < households.size(); i++) {
                        stride::ContactPool* housePool = households[i]->GetPools()[0];
                        if ((current_households_in_community == households_per_community &&
                             (!remainder || current_community >= remainder)) ||
                            (current_households_in_community == households_per_community + 1 &&
                             (remainder && current_community < remainder))) {
                                current_community++;
                                current_households_in_community = 0;
                        }
                        current_households_in_community++;
                        stride::ContactPool* communityPool = community_pools[current_community];
                        for (stride::Person* person : *housePool) {
                                found.insert(communityPool);
                                communityPool->AddMember(person);
                                person->SetSecondaryCommunityId(communityPool->GetId());
                        }
                }
        }

        m_logger->info("Finished populating Secondary Communities");
        m_logger->info("Used {} different Secondary communities", found.size());
}
} // namespace gengeopop
