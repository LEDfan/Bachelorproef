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
 * Header for the core ContactPool class.
 */

#include "contact/ContactLogMode.h"
#include "pool/ContactPoolType.h"

#include <array>
#include <memory>
#include <tuple>
#include <vector>

namespace stride {

class Person;

/**
 * Represents a group of Persons that potentially have contacts.
 */
class ContactPool
{
public:
        using iterator       = std::vector<stride::Person*>::iterator;
        using const_iterator = std::vector<stride::Person*>::const_iterator;

        /// Initializing constructor.
        ContactPool(std::size_t pool_id, ContactPoolType::Id type);

        ContactPool();

        /// Add the given Person.
        void AddMember(const Person* p);

        /// Get member at index.
        Person* GetMember(unsigned int index) const { return m_members[index]; }

        /// Remove a member from the contactpool
        void RemoveMember(Person* pPerson);

        /// Get the entire pool of members.
        const std::vector<Person*>& GetPool() const { return m_members; }

        /// Get size (number of members).
        std::size_t GetSize() const { return m_members.size(); }

        /// Get the pool id
        unsigned int GetId() const;

        /// Get the pool capacity
        std::size_t GetCapacity() const;

        /// Get the pool used capacity, i.e. amount of members
        std::size_t GetUsedCapacity() const;

        /// Get the type of ContactPool, used for logging and tests
        ContactPoolType::Id GetType() const { return m_pool_type; }

        /// Returns the amount of persons and amount of infected persons in this pool
        std::pair<std::size_t, std::size_t> GetPopulationAndInfectedCount() const;

        /// Iterator to first person
        iterator begin() { return m_members.begin(); }

        /// Iterator to end of persons
        iterator end() { return m_members.end(); }

        /// Iterator to first person
        const_iterator begin() const { return cbegin(); }

        /// Iterator to end of persons
        const_iterator end() const { return cend(); }

        /// Iterator to first person
        const_iterator cbegin() const { return m_members.cbegin(); }

        /// Iterator to end of persons
        const_iterator cend() const { return m_members.cend(); }

private:
        /// Sort w.r.t. health status: order: exposed/infected/recovered, susceptible, immune.
        std::tuple<bool, size_t> SortMembers();

        /// Infector calculates contacts and transmissions.
        template <ContactLogMode::Id LL, bool TIC, typename LIP, bool TO>
        friend class Infector;

private:
        std::size_t          m_pool_id;      ///< The ID of the ContactPool (for logging purposes).
        ContactPoolType::Id  m_pool_type;    ///< The type of the ContactPool (for logging purposes).
        std::size_t          m_index_immune; ///< Index of the first immune member in the ContactPool.
        std::vector<Person*> m_members;      ///< Pointers to contactpool members (raw pointers intentional).
        std::size_t          m_capacity;
};

} // namespace stride
