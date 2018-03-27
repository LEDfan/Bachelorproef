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
 * Header file for the core Population class
 */

#include "pop/Person.h"
#include "util/SegmentedVector.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cassert>
#include <exception>
#include <functional>
#include <memory>
#include <typeinfo>
#include <vector>

namespace stride {

/**
 * Container for persons in population.
 */
class Population : public std::vector<Person>
{
public:
        Population() : beliefs_container(){};

        ///
        unsigned int GetAdoptedCount() const;

        /// Get the cumulative number of cases.
        unsigned int GetInfectedCount() const;

        /// New Person in the population.
        void CreatePerson(unsigned int id, double age, unsigned int household_id, unsigned int school_id,
                          unsigned int work_id, unsigned int primary_community_id, unsigned int secondary_community_id,
                          unsigned int start_infectiousness, unsigned int start_symptomatic,
                          unsigned int time_infectious, unsigned int time_symptomatic,
                          const boost::property_tree::ptree& pt_belief, double risk_averseness = 0);

private:
        ///
        template <typename BeliefPolicy>
        void NewPerson(unsigned int id, double age, unsigned int household_id, unsigned int school_id,
                       unsigned int work_id, unsigned int primary_community_id, unsigned int secondary_community_id,
                       unsigned int start_infectiousness, unsigned int start_symptomatic, unsigned int time_infectious,
                       unsigned int time_symptomatic, const boost::property_tree::ptree& pt_belief,
                       double risk_averseness = 0);

        /**
         * A RAII void* for type erasure that can contain anything, owns the pointer, and cleans up
         * Does type verification when NDEBUG is not set
         */
        class Any
        {
        public:
                Any()
                    : m_ptr(nullptr), m_destroy([]() {})
#if not defined(NDEBUG)
                      ,
                      m_id("")
#endif
                {
                }

                template <typename T>
                Any(const T* ptr)
                    : m_ptr(ptr), m_destroy([this]() { delete static_cast<T*>(m_ptr); })
#if not defined(NDEBUG)
                      ,
                      m_id(typeid(T).name())
#endif
                {
                }

                ~Any() { m_destroy(); }

                // We don't need these right now, so this is less work
                Any(const Any&) = delete;
                Any& operator=(const Any&) = delete;
                Any(Any&&)                 = delete;
                Any& operator=(Any&&) = delete;

                template <typename T, class... Args>
                void emplace(Args&&... args)
                {
                        m_destroy();
                        m_ptr     = new T(std::forward<Args>(args)...);
                        m_destroy = [this]() { delete static_cast<T*>(m_ptr); };
#if not defined(NDEBUG)
                        m_id = typeid(T).name();
#endif
                }

                template <typename T>
                T* cast() const
                {
#if not defined(NDEBUG)
                        if (!*this) {
                                throw std::bad_cast();
                        }
                        if (typeid(T).name() != m_id) {
                                throw std::bad_cast();
                        }
#endif
                        return static_cast<T*>(m_ptr);
                }

                operator bool() const { return m_ptr != nullptr; }

        private:
                void*                 m_ptr;
                std::function<void()> m_destroy;
#if not defined(NDEBUG)
                std::string m_id;
#endif
        } beliefs_container; ///< Is either nullptr or util::SegmentedVector<T> where T is the BeliefPolicy NewPerson is
                             ///< called with every time for this instance of Population
};

} // namespace stride
