#include "CommunityGenerator.h"
#include "../Community.h"
#include <iostream>
#include <cmath>
#include <trng/lcg64.hpp>
#include <trng/discrete_dist.hpp>


namespace gengeopop {

CommunityGenerator::CommunityGenerator(stride::util::RNManager &rn_manager) : PartialGenerator(rn_manager) {

}

void CommunityGenerator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig) {
        /*
        * 1. calculate amount of communities, each community has average 2000 persons
        * 2. assign communities to a location by using a discrete distribution which reflects the relative amount of people for that location
        */

        int populationSize = geoGridConfig.populationSize;
        int amountOfCommunities = static_cast<int>(std::ceil(populationSize / 2000.0)); // TODO magic constant

        std::vector<double> weights;

        for (const std::shared_ptr<Location> &loc : *geoGrid) {
                weights.push_back((double) loc->getPopulation() / (double) geoGridConfig.populationSize);
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }


        auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int communityId = 0; communityId < amountOfCommunities; communityId++) {
                int locationId = dist();
                std::shared_ptr<Location> loc = (*geoGrid)[locationId];
                loc->addContactCenter(std::make_shared<Community>());
        }

}

}
