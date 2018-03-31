#include "HouseholdPopulator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/Household.h>
#include <iostream>

namespace gengeopop {

HouseholdPopulator::HouseholdPopulator(stride::util::RNManager& rn_manager) : PartialPopulator(rn_manager) {}

void HouseholdPopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        std::vector<double> weights;

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                weights.push_back(loc->getRelativePopulationSize());
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        unsigned int current_person_id = 0;

        auto loc_dist       = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));
        auto household_dist = m_rnManager.GetGenerator(trng::uniform_int_dist(0, geoGridConfig.household_types.size()));
        for (unsigned int householdId = 0; householdId < geoGridConfig.calc_households; householdId++) {
                int                        locationId      = loc_dist();
                std::shared_ptr<Location>  loc             = (*geoGrid)[locationId];
                int                        householdTypeId = household_dist();
                std::shared_ptr<Household> householdType   = geoGridConfig.household_types[householdTypeId];
                auto                       household = std::make_shared<Household>(geoGridConfig.contactCenters++);
                household->addPool(
                    std::make_shared<ContactPool>(geoGridConfig.contactPools++, householdType->getPoolSize()));
                for (const auto& personType : *householdType->GetPools()[0]) {
                        auto person = std::make_shared<stride::Person>(*personType);
                        // person->SetHouseholdId();
                        // if (person->GetAge < 18) {}
                }
        }
}

} // namespace gengeopop
