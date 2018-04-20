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
        std::cout << std::endl << "Starting to populate Households" << std::endl;
        unsigned int current_person_id = 0;
        auto         household_dist    = m_rnManager.GetGenerator(trng::uniform_int_dist(
            0, static_cast<trng::uniform_int_dist::result_type>(geoGridConfig.generated.household_types.size())));
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                const std::set<std::shared_ptr<ContactCenter>>& households = loc->getContactCentersOfType<Household>();
                for (const auto& household : households) {
                        std::shared_ptr<ContactPool> contactPool     = household->GetPools()[0];
                        auto                         householdTypeId = static_cast<unsigned int>(household_dist());
                        std::shared_ptr<ContactPool> householdType =
                            geoGridConfig.generated.household_types[householdTypeId]->GetPools()[0];
                        for (stride::Person* personType : *householdType) {
                                auto person = geoGrid->CreatePerson(
                                    current_person_id++, personType->GetAge(), household->getId(),
                                    personType->GetSchoolId(), personType->GetWorkId(),
                                    personType->GetPrimaryCommunityId(), personType->GetSecondaryCommunityId());
                                contactPool->addMember(person);
                        }
                }
        }
        std::cout << "Finished populating Households" << std::endl;
        std::cout << "Generated " << current_person_id << " persons" << std::endl;
}

} // namespace gengeopop
