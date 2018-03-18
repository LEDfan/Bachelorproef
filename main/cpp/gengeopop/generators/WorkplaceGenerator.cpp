#include "WorkplaceGenerator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <gengeopop/Workplace.h>
#include <iostream>

namespace gengeopop {

WorkplaceGenerator::WorkplaceGenerator(stride::util::RNManager& rn_manager) : PartialGenerator(rn_manager) {}

void WorkplaceGenerator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        /*
         * 1. the amount of active employees and the amount of commuting people are given
         * 2. calculate the amount of workplaces, each workplace has an average of 20 employees
         * 3. calculate the amount of working people at each location = #residents + #incoming commuting people -
         * #outgoing commuting people
         * 4. use the last information for the distribution
         * 5. assign each workplace to a location
         */

        auto amountOfEmployees = static_cast<int>(
            std::floor(geoGridConfig.populationSize * geoGridConfig.fraction_Workables_WhichAreActive));
        auto amountOfWorkplaces = static_cast<int>(std::ceil(amountOfEmployees / 20.0)); // TODO magic constant

        std::vector<double>
            weights; // = for each location #residents + #incoming commuting people - #outgoing commuting people

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                double amountOfWorkingPeople = loc->getPopulation() * geoGridConfig.fraction_Workables_WhichAreActive +
                                               loc->incomingCommutingPeople(geoGridConfig.fraction_CommutingPeople) -
                                               loc->outGoingCommutingPeople(geoGridConfig.fraction_CommutingPeople);
                weights.push_back(amountOfWorkingPeople / amountOfEmployees);
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int workPlaceId = 0; workPlaceId < amountOfWorkplaces; workPlaceId++) {
                int                       locationId = dist();
                std::shared_ptr<Location> loc        = (*geoGrid)[locationId];
                loc->addContactCenter(std::make_shared<Workplace>());
        }
}

} // namespace gengeopop