#include "CommunityGenerator.h"
#include "../PrimaryCommunity.h"
#include "../SecondaryCommunity.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <iostream>

namespace gengeopop {

void CommunityGenerator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        /*
         * 1. calculate amount of communities, each community has average 2000 persons
         * 2. assign communities to a location by using a discrete distribution which reflects the relative amount of
         * people for that location
         */

        int  populationSize      = geoGridConfig.input.populationSize;
        auto amountOfCommunities = static_cast<int>(std::ceil(populationSize / 2000.0)); // TODO magic constant

        std::vector<double> weights;

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                double weight =
                    static_cast<double>(loc->GetPopulation()) / static_cast<double>(geoGridConfig.input.populationSize);

                CheckWeight("CommunityGenerator", weight);

                weights.push_back(weight);
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int communityId = 0; communityId < amountOfCommunities * 2; communityId++) {
                int                       locationId = dist();
                std::shared_ptr<Location> loc        = (*geoGrid)[locationId];
                if (communityId < amountOfCommunities) {
                        auto community = std::make_shared<PrimaryCommunity>(geoGridConfig.generated.contactCenters++);
                        community->Fill(geoGrid);
                        loc->AddContactCenter(community);
                } else {
                        auto community = std::make_shared<SecondaryCommunity>(geoGridConfig.generated.contactCenters++);
                        community->Fill(geoGrid);
                        loc->AddContactCenter(community);
                }
        }
}

} // namespace gengeopop
