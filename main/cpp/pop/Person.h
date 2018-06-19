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

#include "disease/Health.h"
#include "pool/ContactPoolType.h"
#include "pool/IdSubscriptArray.h"
#include "pool/TravellerIndex.h"
#include "pool/TravellerProfile.h"

#include <boost/property_tree/ptree.hpp>

namespace stride {

class Belief;

class Population;

/**
 * Store and handle person data.
 */
class Person
{
public:
        /// Default construction (for population vector).
        Person()
            : m_id(0), m_age(0.0), m_gender(' '), m_health(), m_is_participant(false),
              m_region(0), m_pool_ids{0, 0, 0, 0, 0, 0}, m_in_pools(false), m_belief(nullptr)
        {
        }

        /// Constructor: set the person data.
        Person(unsigned int id, double age, unsigned int householdId, unsigned int k12SchoolId, unsigned int collegeId,
               unsigned int workId, unsigned int primaryCommunityId, unsigned int secondaryCommunityId,
               std::size_t region, Health health = Health(), double /*risk_averseness*/ = 0, Belief* bp = nullptr)
            : m_id(id), m_age(age), m_gender('M'), m_health(health), m_is_participant(false),
              m_region(region), m_pool_ids{householdId, k12SchoolId,        collegeId,
                                           workId,      primaryCommunityId, secondaryCommunityId},
              m_in_pools(true), m_belief(bp)
        {
                // TODO m_in_pools: shouldn't this check if every id is not 0?
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
        unsigned int GetPoolId(const ContactPoolType::Id& poolType) const { return m_pool_ids[poolType]; }

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
        bool IsInPool(const ContactPoolType::Id poolType, unsigned int poolId) const;

        /// Does this person participates in the social contact study?
        bool IsSurveyParticipant() const { return m_is_participant; }

        /// Participate in social contact study and log person details
        void ParticipateInSurvey() { m_is_participant = true; }

        /// Set the beliefs. Pointer into Population's beliefcontainer.
        void SetBelief(Belief* belief) { m_belief = belief; };

        /// Update the health status and presence in contactpools.
        void Update(bool isWorkOff, bool isSchoolOff, std::shared_ptr<TravellerProfile> travellerProfile,
                    std::shared_ptr<Population> population, std::size_t);

        ///
        void Update(Person* p);

        /// Set the age of the person
        void SetAge(unsigned int newAge);

        unsigned int GetHouseholdId() { return GetPoolId(ContactPoolType::Id::Household); }

        void SetHouseholdId(unsigned int household_id) { SetPoolId(ContactPoolType::Id::Household, household_id); }

        unsigned int GetK12SchoolId() const
        {
                // TODO highschool vs not highschool
                return GetPoolId(ContactPoolType::Id::K12School);
        }

        void SetK12SchoolId(unsigned int school_id)
        {
                // TODO highschool vs not highschool
                SetPoolId(ContactPoolType::Id::K12School, school_id);
        }

        unsigned int GetCollegeId() const
        {
                // TODO highschool vs not highschool
                return GetPoolId(ContactPoolType::Id::College);
        }

        void SetCollegeId(unsigned int highschool_id)
        {
                // TODO highschool vs not highschool
                SetPoolId(ContactPoolType::Id::College, highschool_id);
        }

        unsigned int GetWorkId() const { return GetPoolId(ContactPoolType::Id::Work); }

        void SetWorkId(unsigned int work_id) { SetPoolId(ContactPoolType::Id::Work, work_id); }

        unsigned int GetPrimaryCommunityId() const { return GetPoolId(ContactPoolType::Id::PrimaryCommunity); }

        void SetPrimaryCommunityId(unsigned int primary_community_id)
        {
                SetPoolId(ContactPoolType::Id::PrimaryCommunity, primary_community_id);
        }

        unsigned int GetSecondaryCommunityId() const { return GetPoolId(ContactPoolType::Id::SecondaryCommunity); }

        void SetSecondaryCommunityId(unsigned int secondary_community_id)
        {
                SetPoolId(ContactPoolType::Id::SecondaryCommunity, secondary_community_id);
        }

        void SetPoolId(ContactPoolType::Id type, unsigned int poolId)
        {
                m_pool_ids[type] = poolId;
                m_in_pools[type] = poolId != 0;
        }

        bool IsStudentCandidate() const { return m_age < 18 && m_age >= 6; }

        bool IsCollegeStudentCandidate() const { return m_age >= 18 && m_age < 26; }

        bool IsWorkableCandidate() const { return m_age >= 18 && m_age < 65; }

        /// Returns whether this person is travelling
        bool IsTravelling() const { return m_isTravelling; };

        friend TravellerIndex;

        /// Set the whether the person is travelling
        void SetTravelling(bool travelling);

private:
        unsigned int m_id;             ///< The id.
        double       m_age;            ///< The age.
        char         m_gender;         ///< The gender.
        Health       m_health;         ///< Health info for this person.
        bool         m_is_participant; ///< Is participating in the social contact study
        std::size_t  m_region;         ///< The region

        ///< Ids (school, work, etc) of pools you belong to Id value 0 means you do not belong to any
        ///< pool of that type (e.g. school and work are mutually exclusive.
        ContactPoolType::IdSubscriptArray<unsigned int> m_pool_ids;

        ///< Is person present/absent in pools of each of the types (school, work, etc)?
        ContactPoolType::IdSubscriptArray<bool> m_in_pools;

        Belief* m_belief;               ///< Health beliefs related data (raw pointer intentional).
        bool    m_isTravelling = false; ///< Whether this person is travelling
};

} // namespace stride
