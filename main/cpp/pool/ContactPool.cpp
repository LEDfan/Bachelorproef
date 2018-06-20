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
 * Implementation for the core ContcatPool class.
 */

#include "ContactPool.h"

#include "ContactPoolType.h"
#include "pop/Age.h"
#include "pop/Person.h"

#include <algorithm>

namespace stride {

using namespace std;

ContactPool::ContactPool(std::size_t pool_id, ContactPoolType::Id type)
    : m_pool_id(pool_id), m_pool_type(type), m_index_immune(0), m_members(), m_capacity(), m_expats()
{
}

ContactPool::ContactPool() : m_pool_id(), m_pool_type(), m_index_immune(0), m_members(), m_capacity(), m_expats() {}

void ContactPool::AddMember(const Person* p)
{
        m_members.emplace_back(const_cast<Person*>(p));
        m_index_immune++;
}

std::tuple<bool, size_t> ContactPool::SortMembers()
{
        bool   infectious_cases = false;
        size_t num_cases        = 0;

        for (size_t i_member = 0; i_member < m_index_immune; i_member++) {
                // if immune, move to back
                if (m_members[i_member]->GetHealth().IsImmune()) {
                        bool   swapped   = false;
                        size_t new_place = m_index_immune - 1;
                        m_index_immune--;
                        while (!swapped && new_place > i_member) {
                                if (m_members[new_place]->GetHealth().IsImmune()) {
                                        m_index_immune--;
                                        new_place--;
                                } else {
                                        Swap(i_member, new_place);
                                        swapped = true;
                                }
                        }
                }
                // else, if not susceptible, move to front
                else if (!m_members[i_member]->GetHealth().IsSusceptible()) {
                        if (!infectious_cases && m_members[i_member]->GetHealth().IsInfectious()) {
                                infectious_cases = true;
                        }
                        if (i_member > num_cases) {
                                Swap(i_member, num_cases);
                        }
                        num_cases++;
                }
        }
        return std::make_tuple(infectious_cases, num_cases);
}

std::size_t ContactPool::GetId() const { return m_pool_id; }

std::size_t ContactPool::GetCapacity() const { return m_capacity; }

std::size_t ContactPool::GetUsedCapacity() const { return m_members.size(); }

std::pair<std::size_t, std::size_t> ContactPool::GetPopulationAndInfectedCount() const
{
        unsigned int infected = 0;

        for (stride::Person* person : m_members) {
                if (person->GetHealth().IsInfected()) {
                        infected++;
                }
        }
        return {m_members.size(), infected};
}

void ContactPool::RemoveMember(Person* person)
{
        // use std::find and not std::remove since we need the original position to update the expats
        // note that a person shouldn't be added to a CP more than once
        auto it = std::find(m_members.begin(), m_members.end(), person);
        if (it != m_members.end()) {
                m_members.erase(it);
                UpdateExpatsAfterRemoval(it);
        }
}

void ContactPool::UpdateExpatsAfterRemoval(std::vector<Person*>::iterator itAfterRemovedPerson)
{
        long index = itAfterRemovedPerson - m_members.begin();
        for (std::pair<const Person* const, std::size_t>& expat : m_expats) {
                if (expat.second >= static_cast<std::size_t>(index)) {
                        expat.second--;
                }
                assert(m_members[m_expats[expat.first]] == expat.first);
        }
        if (m_index_immune == m_members.size() + 1) {
                m_index_immune--;
        }
}

void ContactPool::AddExpat(const Person* p)
{
        AddMember(p);
        std::size_t index = m_members.size() - 1;
        m_expats[p]       = index;
        assert(m_members[m_expats[p]] == p);
}

void ContactPool::RemoveExpat(Person* person)
{
        std::size_t index = m_expats.at(person);
        assert(person == m_members[index]);
        auto it = m_members.erase(m_members.begin() + index);
        m_expats.erase(person);

        UpdateExpatsAfterRemoval(it);
}

void ContactPool::Swap(std::size_t person1, std::size_t person2)
{
        std::swap(m_members[person1], m_members[person2]);
        Person* pPerson1 = m_members[person1];
        // Note that a simple check if the person is travelling and is present in this contactpool is not enough
        // to know that it's an expat of this contactpool.
        // E.g. person is travelling and in this contactpool (type work), but as expat in another contactpool (of type
        // primaryCommunity)
        if (m_expats.count(pPerson1)) {
                // m_members[i_member] is an expat -> update map
                m_expats.at(pPerson1) = person1;
                assert(m_members[m_expats[pPerson1]] == pPerson1);
        }
        Person* pPerson2 = m_members[person2];
        if (m_expats.count(pPerson2)) {
                // m_members[i_member] is an expat -> update map
                m_expats.at(pPerson2) = person2;
                assert(m_members[m_expats[pPerson2]] == pPerson2);
        }
}

} // namespace stride
