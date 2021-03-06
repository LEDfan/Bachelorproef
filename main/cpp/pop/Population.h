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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

/**
 * @file
 * Header file for the core Population class
 */

#include "AbstractPopBuilder.h"
#include "ImportPopBuilder.h"
#include "pool/ContactPoolSys.h"
#include "pool/RegionSlicer.h"
#include "pool/TravellerIndex.h"
#include "pop/DefaultPopBuilder.h"
#include "pop/GenPopBuilder.h"
#include "pop/Person.h"
#include "util/Any.h"
#include "util/SegmentedVector.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cassert>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <pool/IdSubscriptArray.h>
#include <spdlog/spdlog.h>
#include <typeinfo>
#include <util/SliceIndexer.h>
#include <vector>

namespace gengeopop {
class GeoGrid;
}

namespace stride {

/**
 * Container for persons in population.
 */
class Population : public util::SegmentedVector<Person>
{
public:
        /// Create a population initialized by the configuration in property tree.
        static std::shared_ptr<Population> Create(const boost::property_tree::ptree& configPt, util::RnMan& rnManager);

        /// For use in python environment: create using configuration string i.o ptree.
        static std::shared_ptr<Population> Create(const std::string& configString, util::RnMan& rnManager);

        /// Create an empty Population with NoBelief policy, used in gengeopop
        static std::shared_ptr<Population> Create();

        ///
        unsigned int GetAdoptedCount() const;

        /// Get the cumulative number of cases.
        unsigned int GetInfectedCount() const;

        ///
        std::shared_ptr<spdlog::logger>& GetContactLogger() { return m_contact_logger; }

        /// The ContactPoolSys of the simulator.
        ContactPoolSys& GetContactPoolSys() { return m_pool_sys; }

        /// The ContactPoolSys of the simulator.
        const ContactPoolSys& GetContactPoolSys() const { return m_pool_sys; }

        std::vector<std::shared_ptr<gengeopop::GeoGrid>> GetGeoGrids() const { return m_geoGrids; }

        /// New Person in the population, the region should be at least the previous region (starting at 0) and at most
        /// 1 larger.
        void CreatePerson(std::size_t regionId, unsigned int id, double age, unsigned int householdId,
                          unsigned int k12SchoolId, unsigned int college, unsigned int workId,
                          unsigned int primaryCommunityId, unsigned int secondaryCommunityId);

        /// Add a new contact pool of a given type in in the given region, the same constraints on the region apply as
        /// for CreatePerson
        ContactPool* CreateContactPool(std::size_t regionId, ContactPoolType::Id typeId);

        /// Get the region identifiers: name -> id
        const std::unordered_map<std::string, std::size_t>& GetRegionIdentifiers() const;

        /// Create a new RegionSlicer for the given region id
        RegionSlicer SliceOnRegion(std::size_t region_id);

        /// Return TravellerIndex for a specific regionId
        TravellerIndex& GetTravellerIndex(std::size_t regionId);

        /// Let the travelling persons return to home
        void ReturnTravellers(std::size_t currentDay);

        /// Returns a boost subrange to the persons of a region
        boost::sliced_range<util::SegmentedVector<Person>>& GetPersonInRegion(std::size_t regionId);

private:
        /// Constructor, to be called by create
        Population();

        /// Initialize beliefs container (including this in SetBeliefPolicy function slows you down
        /// due to guarding against data races in parallel use of SetBeliefPolicy. The DoubleChecked
        /// locking did not work in OpenMP parallel for's on Mac OSX.
        template <typename BeliefPolicy>
        void InitBeliefPolicy()
        {
                if (!m_beliefs) {
                        m_beliefs.emplace<util::SegmentedVector<BeliefPolicy>>(this->size());
                } else {
                        throw std::runtime_error("_func_ : Error, already initialized!");
                }
        }

        /// Assign the belief policy.
        /// \tparam BeliefPolicy Template type param (we could use plain overloading here, i guess)
        /// \param belief        belief object that will be associated with the person
        /// \param i             subscript to person associated with this belief object
        // Cannot follow my preference for declaration of required explicit specializations, because SWIG
        // does not like that. Hence include of the template method definition in the header file.
        template <typename BeliefPolicy>
        void SetBeliefPolicy(std::size_t i, const BeliefPolicy& belief = BeliefPolicy())
        {
                (*this)[i].SetBelief(m_beliefs.cast<util::SegmentedVector<BeliefPolicy>>()->emplace(i, belief));
        }

        /// Create a Population for the given region parameters
        static void CreateRegion(const std::string& geopop_type, const boost::property_tree::ptree& configPt,
                                 const boost::property_tree::ptree& regionPt, const std::shared_ptr<Population>& pop,
                                 const std::string& name, stride::util::RnMan& rnManager);

        /// Update m_currentRegionId and create new ranges in m_pool_sys_regions and m_regionRanges
        void UpdateRegion(std::size_t region_id);

        friend class DefaultPopBuilder;
        friend class GenPopBuilder;
        friend class ImportPopBuilder;
        friend class BeliefSeeder;

        // Placed separately to avoid overly long declarations
        using ContactPoolSysRanges =
            ContactPoolType::IdSubscriptArray<util::SliceIndexer<util::SegmentedVector<ContactPool>, std::size_t>>;

        boost::property_tree::ptree m_belief_pt;        ///< Belief configuration
        util::Any                   m_beliefs;          ///< Holds belief data for the persons.
        ContactPoolSys              m_pool_sys;         ///< Holds vector of ContactPools of different types.
        ContactPoolSysRanges        m_pool_sys_regions; ///< Holds sub_ranges for region indexin, by contactpool type
        std::shared_ptr<spdlog::logger>                  m_contact_logger; ///< Logger for contact/transmission.
        std::vector<std::shared_ptr<gengeopop::GeoGrid>> m_geoGrids;       ///< Associated geoGrid may be nullptr
        util::SliceIndexer<util::SegmentedVector<Person>, std::size_t>
            m_regionRanges; ///< Ranges over the people in different regions
        std::vector<TravellerIndex>
                                                     m_regionTravellerIndex; ///< For each region keep a TravellerIndex which holds the travelling information
        std::unordered_map<std::string, std::size_t> m_regions; ///< Regios

        // Cannot make negative because size_t is unsigned, special check needed in the Create methods
        std::size_t m_currentRegionId      = 0;     ///< Keep track of the last seen region id
        bool        m_have_inserted        = false; ///< Keep track whether the first Region was created yet
        std::size_t m_currentContactPoolId = 1;     ///< The current contact pool id, assigns in increasing order
};

} // namespace stride
