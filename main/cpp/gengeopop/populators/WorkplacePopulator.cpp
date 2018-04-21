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

WorkplacePopulator::WorkplacePopulator(stride::util::RNManager& rn_manager, std::shared_ptr<spdlog::logger> logger)
    : PartialPopulator(rn_manager, logger),
      m_fractionCommutingStudents(0), m_workplacesInCity{}, m_currentLoc{}, m_geoGridConfig{}, m_geoGrid{}
{
}

void WorkplacePopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        m_logger->info("Starting to populate Workplaces");

        m_geoGrid                   = geoGrid;
        m_geoGridConfig             = geoGridConfig;
        m_fractionCommutingStudents = 0;
        m_workplacesInCity.clear();
        m_currentLoc           = nullptr;
        m_assignedTo0          = 0;
        m_assignedCommuting    = 0;
        m_assignedNotCommuting = 0;
        m_distNonCommuting     = discreteDist();
        m_nearByWorkplaces.clear();
        m_disCommuting = discreteDist();
        m_commutingLocations.clear();

        calculateFractionCommutingStudents();
        calculateWorkplacesInCity();

        // for every location
        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                m_currentLoc = loc;
                calculateCommutingLocations();
                calculateNearbyWorkspaces();

                // 2. for every worker assign a class
                for (const std::shared_ptr<ContactCenter>& household : loc->getContactCentersOfType<Household>()) {
                        const std::shared_ptr<ContactPool>& contactPool = household->GetPools()[0];
                        for (stride::Person* person : *contactPool) {
                                if ((person->GetAge() >= 18 && person->GetAge() < 65)) {

                                        bool isStudent =
                                            MakeChoice(geoGridConfig.input.fraction_1826_years_WhichAreStudents);
                                        bool isActiveWorker =
                                            MakeChoice(geoGridConfig.input.fraction_1865_years_active);

                                        if ((person->GetAge() >= 18 && person->GetAge() < 26 && !isStudent) ||
                                            isActiveWorker) {
                                                AssignActive(person);
                                        } else {
                                                // this person isn't an active employee
                                                person->SetWorkId(0);
                                                m_assignedTo0++;
                                        }
                                }
                        }
                }
        }

        m_logger->info("Populated workplaces, assigned to 0 {}, assigned (commuting) {} assigned (not commuting) {} ",
                       m_assignedTo0, m_assignedCommuting, m_assignedNotCommuting);
}

void WorkplacePopulator::calculateFractionCommutingStudents()
{
        m_fractionCommutingStudents =
            (m_geoGridConfig.calculated._1826_years_and_student *
             m_geoGridConfig.input.fraction_student_commutingPeople) /
            (m_geoGridConfig.calculated._1865_and_years_active * m_geoGridConfig.input.fraction_active_commutingPeople);
}
void WorkplacePopulator::calculateWorkplacesInCity()
{
        for (const std::shared_ptr<Location>& loc : *m_geoGrid) {
                const auto&                               Workplaces = loc->getContactCentersOfType<Workplace>();
                std::vector<std::shared_ptr<ContactPool>> contactPools;

                for (const auto& Workplace : Workplaces) {
                        contactPools.insert(contactPools.end(), Workplace->begin(), Workplace->end());
                }

                auto disPools = m_rnManager.GetGenerator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(contactPools.size())));

                m_workplacesInCity[loc] = {contactPools, disPools};
        }
}

void WorkplacePopulator::AssignActive(stride::Person* person)
{
        // this person is (student and active) or active
        if (!m_commutingLocations.empty() && MakeChoice(m_geoGridConfig.input.fraction_active_commutingPeople)) {
                // this person is commuting

                // id of the location this person is commuting to
                auto        locationId = m_disCommuting();
                const auto& info       = m_workplacesInCity[m_commutingLocations[locationId]];
                auto        id         = info.second();

                info.first[id]->addMember(person);
                person->SetWorkId(static_cast<unsigned int>(id));
                m_assignedCommuting++;
        } else {
                auto id = m_distNonCommuting();
                m_nearByWorkplaces[id]->addMember(person);
                person->SetWorkId(static_cast<unsigned int>(id));
                m_assignedNotCommuting++;
        }
}

void WorkplacePopulator::calculateCommutingLocations()
{
        // find all Workplaces were employees from this location commute to
        m_commutingLocations.clear();
        m_disCommuting = discreteDist();

        std::vector<double> commutingWeights;
        for (const std::pair<std::shared_ptr<Location>, double>& commute : m_currentLoc->getOutgoingCommuningCities()) {
                const auto& Workplaces = commute.first->getContactCentersOfType<Workplace>();
                if (!Workplaces.empty()) {
                        m_commutingLocations.push_back(commute.first);
                        commutingWeights.push_back(commute.second - (commute.second * m_fractionCommutingStudents));
                }
        }

        if (!commutingWeights.empty()) {
                m_disCommuting =
                    m_rnManager.GetGenerator(trng::discrete_dist(commutingWeights.begin(), commutingWeights.end()));
        }
}

void WorkplacePopulator::calculateNearbyWorkspaces()
{
        m_nearByWorkplaces = GetContactPoolInIncreasingRadius<Workplace>(m_geoGrid, m_currentLoc);
        m_distNonCommuting = m_rnManager.GetGenerator(
            trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(m_nearByWorkplaces.size())));
}

} // namespace gengeopop
