#include "HouseholdGenerator.h"
#include "gengeopop/K12School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <iostream>

namespace gengeopop {

void HouseholdGenerator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        std::vector<double> weights;

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                weights.push_back(loc->GetRelativePopulationSize());
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        auto loc_dist = m_rnManager[0].variate_generator(trng::discrete_dist(weights.begin(), weights.end()));
        for (unsigned int householdId = 0; householdId < geoGridConfig.calculated.households; householdId++) {
                int                       locationId = loc_dist();
                std::shared_ptr<Location> loc        = (*geoGrid)[locationId];
                auto household = std::make_shared<Household>(geoGridConfig.generated.contactCenters++);
                household->Fill(geoGrid);
                loc->AddContactCenter(household);
        }
}

} // namespace gengeopop
