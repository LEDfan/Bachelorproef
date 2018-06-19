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
 * Header file for the Person class.
 */

#include "Person.h"
#include "Population.h"
#include <pool/TravellerProfile.h>

#include "pool/ContactPoolType.h"
#include "pop/Age.h"

namespace stride {

using namespace std;
using namespace stride::ContactPoolType;

void Person::Update(bool isWorkOff, bool isSchoolOff, std::shared_ptr<TravellerProfile> travellerProfile,
                    std::shared_ptr<Population> population)
{
        m_health.Update();

        // Update presence in contactpools.
        if (m_health.IsSymptomatic()) {
                m_in_pools[Id::K12School]          = false;
                m_in_pools[Id::College]            = false;
                m_in_pools[Id::Work]               = false;
                m_in_pools[Id::PrimaryCommunity]   = false;
                m_in_pools[Id::SecondaryCommunity] = false;
        } else if (isWorkOff || (m_age <= MinAdultAge() && isSchoolOff)) {
                m_in_pools[Id::K12School]          = false;
                m_in_pools[Id::College]            = false;
                m_in_pools[Id::Work]               = false;
                m_in_pools[Id::PrimaryCommunity]   = true;
                m_in_pools[Id::SecondaryCommunity] = false;
        } else {
                m_in_pools[Id::K12School]          = true;
                m_in_pools[Id::College]            = true;
                m_in_pools[Id::Work]               = true;
                m_in_pools[Id::PrimaryCommunity]   = false;
                m_in_pools[Id::SecondaryCommunity] = true;
        }

        /**
         * This must be refactored to increase the performance and incorporate the proposal from the documents.
         */
        if (m_isTravelling) {
                m_travelDaysRemaining--;
                if (m_travelDaysRemaining == 0) {
                        m_isTravelling = false;
                        if (m_visitingContactPool) {
                                m_visitingContactPool->RemoveMember(this);
                        }
                }
        } else {
                const auto& travelData = travellerProfile->PersonWillTravel(m_region);
                if (std::get<0>(travelData)) {
                        m_in_pools[Id::K12School]          = false;
                        m_in_pools[Id::College]            = false;
                        m_in_pools[Id::Work]               = false;
                        m_in_pools[Id::PrimaryCommunity]   = false;
                        m_in_pools[Id::SecondaryCommunity] = false;
                        if (std::get<1>(travelData)) {
                                // work
                                m_visitingContactPool = population->GetWorkInRegion(std::get<2>(travelData));
                        } else {
                                // not work
                                m_visitingContactPool =
                                    population->GetPrimaryCommunityInRegion(std::get<2>(travelData));
                        }
                        m_isTravelling        = true;
                        m_travelDaysRemaining = std::get<3>(travelData);
                }
        }
}

void Person::Update(Person*)
{
        // TODO update beliefs
}

void Person::SetAge(unsigned int newAge) { m_age = newAge; }

} // namespace stride
