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
#include <unordered_map>
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

        /// Add the given Person as an expat.
        void AddExpat(const Person* p);

        /// Get member at index.
        Person* GetMember(unsigned int index) const { return m_members[index]; }

        /// Remove a member from the contactpool
        void RemoveMember(Person* pPerson);

        /// Remove an expat from the contactpool
        void RemoveExpat(Person* Person);

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

        /// Returns the amount of perons and amount of infected perons in this pol
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

        /// Sort w.r.t. health status: order: exposed/infected/recovered, susceptible, immune.
        std::tuple<bool, size_t> SortMembers();

public:
        /// Infector calculates contacts and transmissions.
        template <ContactLogMode::Id LL, bool TIC, typename LIP, bool TO>
        friend class Infector;

private:
        unsigned int         m_pool_id;      ///< The ID of the ContactPool (for logging purposes).
        ContactPoolType::Id  m_pool_type;    ///< The type of the ContactPool (for logging purposes).
        std::size_t          m_index_immune; ///< Index of the first immune member in the ContactPool.
        std::vector<Person*> m_members;      ///< Pointers to contactpool members (raw pointers intentional).
        std::size_t          m_capacity;

        /// Helper data structure for fast deletions of expats from m_members
        /// Can't keep them in the end of m_members and do a pop_back() since \ref SortMembers will sort the members.
        /// Also can't prevent this from happening in SortMembers, since this would break the logic in \ref Infector
        /// Will be updated in \ref SortMembers.
        std::unordered_map<const Person*, std::size_t> m_expats;

        /// Update the m_expats data after removing a element from m_members
        /// \p itAfterRemovedPerson is the iterator returned by std::find or std::erase
        void UpdateExpatsAfterRemoval(std::vector<Person*>::iterator itAfterRemovedPerson);

        /// Swaps persons in m_members with the given index and updates the expats data if necessary
        void Swap(std::size_t person1, std::size_t person2);
};

} // namespace stride
