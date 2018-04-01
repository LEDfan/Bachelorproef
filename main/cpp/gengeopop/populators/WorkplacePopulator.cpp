#include "WorkplacePopulator.h"
#include "../Workplace.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <trng/uniform_int_dist.hpp>
#include <cmath>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/Workplace.h>
#include <iostream>
#include <pop/Person.h>

namespace gengeopop {

WorkplacePopulator::WorkplacePopulator(stride::util::RNManager& rn_manager) : PartialPopulator(rn_manager) {}

void WorkplacePopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{

        std::cout << std::endl << "Starting to populate Workplaces" << std::endl;
        double fraction =
            (geoGridConfig.calculated._1826_years_and_student * geoGridConfig.input.fraction_commutingPeople) /
            (geoGridConfig.calculated._1865_and_years_active *
             geoGridConfig.input.fraction_commutingPeople); // the fraction_commutingPeople is included here, because it
                                                            // may be splitted into students and workers later on

        unsigned int assignedTo0          = 0;
        unsigned int assignedCommuting    = 0;
        unsigned int assignedNotCommuting = 0;

        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                // 1. find all Workplaces in an area of 10-k*10 km
                std::vector<std::shared_ptr<ContactPool>> nearByWorkplaces =
                    GetContactPoolInIncreasingRadius<Workplace>(geoGrid, loc);
                auto distNonCommuting = m_rnManager.GetGenerator(trng::uniform_int_dist(
                    0, static_cast<trng::uniform_int_dist::result_type>(nearByWorkplaces.size())));

                // 2. find all Workplaces were employees from this location commute to
                std::vector<std::shared_ptr<Location>> commutingWorkplaces;
                std::vector<double>                    commutingWeights;
                for (const std::pair<std::shared_ptr<Location>, double>& commute : loc->getOutgoingCommuningCities()) {
                        auto Workplaces = commute.first->getContactCentersOfType<Workplace>();
                        if (!Workplaces.empty()) {
                                commutingWorkplaces.push_back(commute.first);
                                commutingWeights.push_back(commute.second - (commute.second * fraction));
                        }
                }

                std::function<trng::discrete_dist::result_type()> disCommuting;

                if (!commutingWeights.empty()) {
                        disCommuting = m_rnManager.GetGenerator(
                            trng::discrete_dist(commutingWeights.begin(), commutingWeights.end()));
                }

                // 2. for every worker assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->getContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool> contactPool = household->GetPools()[0];
                        for (const std::shared_ptr<stride::Person>& person : *contactPool) {
                                if (person->GetAge() >= 18 && person->GetAge() < 65) {
                                        if (MakeChoice(geoGridConfig.input.fraction_1826_years_WhichAreStudents)) {
                                                // this person is an active employee
                                                if (!commutingWorkplaces.empty() &&
                                                    MakeChoice(geoGridConfig.input.fraction_commutingPeople)) {
                                                        // this person is commuting

                                                        // id of the location this person is commuting to
                                                        auto locationId = disCommuting();
                                                        // create a list of ContactPools (i.e. classes) for each of
                                                        // Workplace of this location
                                                        auto Workplaces = commutingWorkplaces[locationId]
                                                                              ->getContactCentersOfType<Workplace>();
                                                        std::vector<std::shared_ptr<ContactPool>> contactPools;
                                                        for (const auto& Workplace : Workplaces) {
                                                                contactPools.insert(contactPools.end(),
                                                                                    Workplace->begin(),
                                                                                    Workplace->end());
                                                        }

                                                        auto disPools = m_rnManager.GetGenerator(trng::uniform_int_dist(
                                                            0, static_cast<trng::uniform_int_dist::result_type>(
                                                                   contactPools.size())));

                                                        auto id = disPools();
                                                        contactPools[id]->addMember(person);
                                                        person->SetWorkId(static_cast<unsigned int>(id));
                                                        assignedCommuting++;
                                                } else {
                                                        auto id = distNonCommuting();
                                                        nearByWorkplaces[id]->addMember(person);
                                                        person->SetWorkId(static_cast<unsigned int>(id));
                                                        assignedNotCommuting++;
                                                }
                                        } else {
                                                // this person isn't an active employee
                                                person->SetWorkId(0);
                                                assignedTo0++;
                                        }
                                }
                        }
                }
        }
        std::cout << "Populated workplaces, assigned to 0 " << assignedTo0 << ", assigned (commuting) "
                  << assignedCommuting << ", assigned (not commuting) " << assignedNotCommuting << std::endl;
}

} // namespace gengeopop
