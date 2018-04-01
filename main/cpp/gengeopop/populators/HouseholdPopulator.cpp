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
        unsigned int current_person_id = 0;
        auto         household_dist =
            m_rnManager.GetGenerator(trng::uniform_int_dist(0, geoGridConfig.generated.household_types.size()));
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                std::set<std::shared_ptr<ContactCenter>> households = loc->getContactCentersOfType<Household>();
                for (const auto& household : households) {
                        std::shared_ptr<ContactPool> contactPool     = household->GetPools()[0];
                        unsigned int                 householdTypeId = household_dist();
                        std::shared_ptr<ContactPool> householdType =
                            geoGridConfig.generated.household_types[householdTypeId]->GetPools()[0];
                        for (const std::shared_ptr<stride::Person>& personType : *householdType) {
                                auto person = std::make_shared<stride::Person>(*personType);
                                person->SetId(current_person_id++);
                                person->SetHouseholdId(household->getId());
                                contactPool->addMember(person);
                        }
                }
        }
}

} // namespace gengeopop
