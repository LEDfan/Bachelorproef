#include "HouseholdPopulator.h"
#include "gengeopop/K12School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/Household.h>
#include <iostream>

namespace gengeopop {

void HouseholdPopulator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        m_logger->info("Starting to populate Households");

        unsigned int current_person_id = 0;
        auto         household_dist    = m_rnManager.GetGenerator(trng::uniform_int_dist(
            0, static_cast<trng::uniform_int_dist::result_type>(geoGridConfig.generated.household_types.size())));
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                const std::vector<std::shared_ptr<ContactCenter>>& households =
                    loc->GetContactCentersOfType<Household>();
                for (const auto& household : households) {
                        stride::ContactPool* contactPool     = household->GetPools()[0];
                        auto                 householdTypeId = static_cast<unsigned int>(household_dist());
                        stride::ContactPool* householdType =
                            geoGridConfig.generated.household_types[householdTypeId]->GetPools()[0];
                        for (stride::Person* personType : *householdType) {
                                auto person = geoGrid->CreatePerson(current_person_id++, personType->GetAge(),
                                                                    contactPool->GetId(), 0, 0, 0, 0, 0);
                                contactPool->AddMember(person);
                        }
                }
        }
        m_logger->info("Finished populating Households");
        m_logger->info("Generated {} persons", current_person_id);
}

} // namespace gengeopop
