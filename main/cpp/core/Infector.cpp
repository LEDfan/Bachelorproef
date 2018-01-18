/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Implementation of Infector algorithms.
 */

#include "Infector.h"

#include "core/Cluster.h"

#include <spdlog/spdlog.h>

namespace stride {

using namespace std;

/// Primary R0_POLICY: do nothing i.e. track all cases.
/// \tparam TIC         TrackIndexCase
template <bool TIC>
class R0_POLICY
{
public:
        static void Exec(Person* p) {}
};

/// Specialized R0_POLICY: track only the index case.
template <>
class R0_POLICY<true>
{
public:
        static void Exec(Person* p) { p->GetHealth().StopInfection(); }
};

/// Primary LOG_POLICY policy, implements LogMode::None.
/// \tparam LL
template <LogMode::Id LL>
class LOG_POLICY
{
public:
        static void Contact(const shared_ptr<spdlog::logger>& logger, Person* p1, Person* p2,
                            ClusterType::Id cluster_type, const shared_ptr<const Calendar>& environ)
        {
        }

        static void Transmission(const shared_ptr<spdlog::logger>& logger, Person* p1, Person* p2,
                                 ClusterType::Id cluster_type, const shared_ptr<const Calendar>& calendar)
        {
        }
};

/// Specialized LOG_POLICY policy LogMode::Transmissions.
template <>
class LOG_POLICY<LogMode::Id::Transmissions>
{
public:
        static void Contact(const shared_ptr<spdlog::logger>& logger, Person* p1, Person* p2,
                            ClusterType::Id cluster_type, const shared_ptr<const Calendar>& environ)
        {
        }

        static void Transmission(const shared_ptr<spdlog::logger>& logger, Person* p1, Person* p2,
                                 ClusterType::Id cluster_type, const shared_ptr<const Calendar>& calendar)
        {
                logger->info("[TRAN] {} {} {} {}", p1->GetId(), p2->GetId(), ClusterType::ToString(cluster_type),
                             calendar->GetSimulationDay());
        }
};

/// Specialized LOG_POLICY policy LogMode::Contacts.
template <>
class LOG_POLICY<LogMode::Id::Contacts>
{
public:
        static void Contact(const shared_ptr<spdlog::logger>& logger, Person* p1, Person* p2,
                            ClusterType::Id cluster_type, const shared_ptr<const Calendar>& calendar)
        {
                const auto home = (cluster_type == ClusterType::Id::Household);
                const auto work = (cluster_type == ClusterType::Id::Work);
                const auto school = (cluster_type == ClusterType::Id::School);
                const auto primary_community = (cluster_type == ClusterType::Id::PrimaryCommunity);
                const auto secundary_community = (cluster_type == ClusterType::Id::SecondaryCommunity);

                logger->info("[CONT] {} {} {} {} {} {} {} {} {}", p1->GetId(), p1->GetAge(), p2->GetAge(),
                             static_cast<unsigned int>(home), static_cast<unsigned int>(school),
                             static_cast<unsigned int>(work), static_cast<unsigned int>(primary_community),
                             static_cast<unsigned int>(secundary_community), calendar->GetSimulationDay());
        }

        static void Transmission(const shared_ptr<spdlog::logger>& logger, Person* p1, Person* p2,
                                 ClusterType::Id cluster_type, const shared_ptr<const Calendar>& calendar)
        {
                logger->info("[TRAN] {} {} {} {}", p1->GetId(), p2->GetId(), ClusterType::ToString(cluster_type),
                             calendar->GetSimulationDay());
        }
};

//-------------------------------------------------------------------------------------------------
// Definition for primary template covers the situation for LogMode::None & LogMode::Transmissions,
// both with track_index_case false and true.
// And every local information policy except NoLocalInformation
//-------------------------------------------------------------------------------------------------
template <LogMode::Id LL, bool TIC, typename LIP, bool TO>
void Infector<LL, TIC, LIP, TO>::Exec(Cluster& cluster, DiseaseProfile disease_profile, RngHandler& contact_handler,
                                      shared_ptr<const Calendar> calendar)
{
        using LP = LOG_POLICY<LL>;
        using RP = R0_POLICY<TIC>;

        cluster.UpdateMemberPresence();

        // set up some stuff
        auto logger = spdlog::get("contact_logger");
        const auto c_type = cluster.m_cluster_type;
        const auto& c_members = cluster.m_members;
        const auto t_rate = disease_profile.GetTransmissionRate();

        // check all contacts
        for (size_t i_person1 = 0; i_person1 < c_members.size(); i_person1++) {
                // check if member is present today
                if (c_members[i_person1].second) {
                        auto p1 = c_members[i_person1].first;
                        const double c_rate = cluster.GetContactRate(p1);
                        // loop over possible contacts (contacts can be initiated by each member)
                        for (const auto& member : c_members) {
                                // check if member is present today
                                if (member.second) {
                                        auto p2 = member.first;
                                        // check for contact
                                        if (contact_handler.HasContact(c_rate)) {
                                                // log contact, if person 1 is participating in survey
                                                if (p1->IsParticipatingInSurvey()) {
                                                        LP::Contact(logger, p1, p2, c_type, calendar);
                                                }
                                                // log contact, if person 2 is participating in survey
                                                if (p2->IsParticipatingInSurvey()) {
                                                        LP::Contact(logger, p2, p1, c_type, calendar);
                                                }
                                                // exchange information about health state & beliefs
                                                LIP::Update(p1, p2);
                                                // transmission & insfection.
                                                if (contact_handler.HasTransmission(t_rate)) {
                                                        if (p1->GetHealth().IsInfectious() &&
                                                            p2->GetHealth().IsSusceptible()) {
                                                                LP::Transmission(logger, p1, p2, c_type, calendar);
                                                                p2->GetHealth().StartInfection();
                                                                RP::Exec(p2);
                                                        } else if (p2->GetHealth().IsInfectious() &&
                                                                   p1->GetHealth().IsSusceptible()) {
                                                                LP::Transmission(logger, p2, p1, c_type, calendar);
                                                                p1->GetHealth().StartInfection();
                                                                RP::Exec(p1);
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
}

//-------------------------------------------------------------------------------------------
// Time optimized implementation for NoLocalInformationPolicy and None || Transmission logging.
//-------------------------------------------------------------------------------------------
template <LogMode::Id LL, bool TIC>
void Infector<LL, TIC, NoLocalInformation, true>::Exec(Cluster& cluster, DiseaseProfile disease_profile, RngHandler& ch,
                                                       shared_ptr<const Calendar> calendar)
{
        using LP = LOG_POLICY<LL>;
        using RP = R0_POLICY<TIC>;

        // check if the cluster has infected members and sort
        bool infectious_cases;
        size_t num_cases;
        tie(infectious_cases, num_cases) = cluster.SortMembers();

        if (infectious_cases) {
                cluster.UpdateMemberPresence();

                // set up some stuff
                auto logger = spdlog::get("contact_logger");
                const auto c_type = cluster.m_cluster_type;
                const auto c_immune = cluster.m_index_immune;
                const auto& c_members = cluster.m_members;
                const auto t_rate = disease_profile.GetTransmissionRate();

                // match infectious and susceptible members, skip last part (immune members)
                for (size_t i_infected = 0; i_infected < num_cases; i_infected++) {
                        // check if member is present today
                        if (c_members[i_infected].second) {
                                const auto p1 = c_members[i_infected].first;
                                if (p1->GetHealth().IsInfectious()) {
                                        const double c_rate_p1 = cluster.GetContactRate(p1);
                                        // loop over possible susceptible contacts
                                        for (size_t i_contact = num_cases; i_contact < c_immune; i_contact++) {
                                                // check if member is present today
                                                if (c_members[i_contact].second) {
                                                        auto p2 = c_members[i_contact].first;
                                                        const double c_rate_p2 = cluster.GetContactRate(p2);
                                                        if (ch.HasContactAndTransmission(c_rate_p1, t_rate) ||
                                                            ch.HasContactAndTransmission(c_rate_p2, t_rate)) {
                                                                if (p1->GetHealth().IsInfectious() &&
                                                                    p2->GetHealth().IsSusceptible()) {
                                                                        p2->GetHealth().StartInfection();
                                                                        RP::Exec(p2);
                                                                        LP::Transmission(logger, p1, p2, c_type,
                                                                                         calendar);
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
}

//--------------------------------------------------------------------------
// All explicit instantiations.
//--------------------------------------------------------------------------
template class Infector<LogMode::Id::None, false, NoLocalInformation>;
template class Infector<LogMode::Id::None, false, LocalDiscussion>;
template class Infector<LogMode::Id::None, true, NoLocalInformation>;
template class Infector<LogMode::Id::None, true, LocalDiscussion>;

template class Infector<LogMode::Id::Transmissions, false, NoLocalInformation>;
template class Infector<LogMode::Id::Transmissions, false, LocalDiscussion>;
template class Infector<LogMode::Id::Transmissions, true, NoLocalInformation>;
template class Infector<LogMode::Id::Transmissions, true, LocalDiscussion>;

template class Infector<LogMode::Id::Contacts, false, NoLocalInformation>;
template class Infector<LogMode::Id::Contacts, false, LocalDiscussion>;
template class Infector<LogMode::Id::Contacts, true, NoLocalInformation>;
template class Infector<LogMode::Id::Contacts, true, LocalDiscussion>;

} // namespace stride
