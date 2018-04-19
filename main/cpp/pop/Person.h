#pragma once
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

#include "behaviour/belief_policies/Belief.h"
#include "disease/Health.h"
#include "pool/ContactPoolType.h"
#include "pool/IdSubscriptArray.h"

#include <boost/property_tree/ptree.hpp>

namespace stride {

/**
 * Store and handle person data.
 */
class Person
{
public:
        /// Default construction.
        Person()
            : m_id(0), m_age(0.0), m_gender(' '), m_health(), m_is_participant(false), m_pool_ids{0U},
              m_in_pools(false), m_belief(nullptr), m_household_id(0), m_school_id(0), m_highschool_id(0), m_work_id(0),
              m_primary_community_id(0), m_secondary_community_id(0)
        {
        }

        /// Constructor: set the person data.
        Person(unsigned int id, double age, unsigned int household_id, unsigned int school_id, unsigned int work_id,
               unsigned int primary_community_id, unsigned int secondary_community_id, Health health = Health(),
               double /*risk_averseness*/ = 0, Belief* bp = nullptr)
            : m_id(id), m_age(age), m_gender('M'), m_health(health),
              m_is_participant(false), m_pool_ids{household_id, school_id, work_id, primary_community_id,
                                                  secondary_community_id},
              m_in_pools(true), m_belief(bp), m_household_id(household_id), m_school_id(school_id), m_highschool_id(0),
              m_work_id(work_id), m_primary_community_id(primary_community_id),
              m_secondary_community_id(secondary_community_id)
        {
                // TODO highSchooldid?
        }

        /// Is this person not equal to the given person?
        bool operator!=(const Person& p) const { return p.m_id != m_id; }

        /// Get the age.
        double GetAge() const { return m_age; }

        /// Return belief info.
        Belief* GetBelief() { return m_belief; }

        /// Return belief info.
        const Belief* GetBelief() const { return m_belief; }

        /// Get ID of contactpool_type
        unsigned int GetPoolId(const ContactPoolType::Id& pool_type) const { return m_pool_ids[pool_type]; }

        /// Return person's gender.
        char GetGender() const { return m_gender; }

        /// Return person's health status.
        Health& GetHealth() { return m_health; }

        /// Return person's health status.
        const Health& GetHealth() const { return m_health; }

        /// Get the id.
        unsigned int GetId() const { return m_id; }

        /// Set the id.
        void SetId(unsigned int id) { m_id = id; }

        /// Check if a person is present today in a given contactpool
        bool IsInPool(const ContactPoolType::Id& pool_type) const { return m_in_pools[pool_type]; }

        /// Does this person participates in the social contact study?
        bool IsParticipatingInSurvey() const { return m_is_participant; }

        /// Participate in social contact study and log person details
        void ParticipateInSurvey() { m_is_participant = true; }

        /// Update the health status and presence in contactpools.
        void Update(bool is_work_off, bool is_school_off);

        ///
        void Update(Person* p);

        /// Set the age of the person
        void SetAge(unsigned int newAge);

        unsigned int GetHouseholdId() { return m_household_id; }

        void SetHouseholdId(unsigned int household_id) { m_household_id = household_id; }

        unsigned int GetSchoolId() { return m_school_id; }

        void SetSchoolId(unsigned int school_id) { m_school_id = school_id; }

        unsigned int GetHighSchoolId() { return m_highschool_id; };

        void SetHighSchoolId(unsigned int highschool_id) { m_highschool_id = highschool_id; };

        unsigned int GetWorkId() { return m_work_id; }

        void SetWorkId(unsigned int work_id) { m_work_id = work_id; }

        unsigned int GetPrimaryCommunityId() { return m_primary_community_id; }

        void SetPrimaryCommunityId(unsigned int primary_community_id) { m_primary_community_id = primary_community_id; }

        unsigned int GetSecondaryCommunityId() { return m_secondary_community_id; }

        void SetSecondaryCommunityId(unsigned int secondary_community_id)
        {
                m_secondary_community_id = secondary_community_id;
        }

private:
        unsigned int m_id;     ///< The id.
        double       m_age;    ///< The age.
        char         m_gender; ///< The gender.

        Health m_health; ///< Health info for this person.

        bool m_is_participant; ///< Is participating in the social contact study
                               //        bool m_at_home_due_to_illness; ///< Is person present home due to illness?
        ContactPoolType::IdSubscriptArray<unsigned int> m_pool_ids; ///< Ids (school, work, etc) of pools you belong to.
                                                                    ///< Id value 0 means you do not belong to any
                                                                    ///< pool of that type (e.g. school and work are
                                                                    ///< mutually exclusive.

        ContactPoolType::IdSubscriptArray<bool> m_in_pools; ///< Is person present/absent in pools of each of the
                                                            ///< types (school, work, etc)?

        Belief* m_belief; ///< Health beliefs related data.

        unsigned int m_household_id;           ///< The household id.
        unsigned int m_school_id;              ///< The school contactpool id
        unsigned int m_highschool_id;          ///< The highschool contactpool id
        unsigned int m_work_id;                ///< The work contactpool id
        unsigned int m_primary_community_id;   ///< The primary community id
        unsigned int m_secondary_community_id; ///< The secondary community id
};

} // namespace stride
