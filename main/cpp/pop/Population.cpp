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
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Core Population class
 */

#include "Population.h"
#include <pool/ContactPoolType.h>

#include "behaviour/belief_policies/Imitation.h"
#include "behaviour/belief_policies/NoBelief.h"
#include "pop/DefaultPopBuilder.h"
#include "pop/GenPopBuilder.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/RNManager.h"
#include "util/RunConfigManager.h"

using namespace boost::property_tree;
using namespace std;
using namespace stride::util;

namespace stride {

std::shared_ptr<Population> Population::Create(const boost::property_tree::ptree& configPt)
{
        // --------------------------------------------------------------
        // Create (empty) population & and give it a ContactLogger.
        // --------------------------------------------------------------
        struct make_shared_enabler : public Population
        {
        };
        auto pop = make_shared<make_shared_enabler>();
        if (configPt.get<bool>("run.contact_output_file", true)) {
                const auto prefix       = configPt.get<string>("run.output_prefix");
                const auto logPath      = FileSys::BuildPath(prefix, "contact_log.txt");
                pop->GetContactLogger() = LogUtils::CreateRotatingLogger("contact_logger", logPath.string());
                pop->GetContactLogger()->set_pattern("%v");
        } else {
                pop->GetContactLogger() = LogUtils::CreateNullLogger("contact_logger");
        }

        pop->m_belief_pt = configPt.get_child("run.belief_policy");

        // ------------------------------------------------
        // Setup RNManager.
        // ------------------------------------------------
        RNManager rnManager(RNManager::Info{configPt.get<string>("pop.rng_type", "lcg64"),
                                            configPt.get<unsigned long>("pop.rng_seed", 101UL), "",
                                            configPt.get<unsigned int>("run.num_threads")});

        auto stride_logger = spdlog::get("stride_logger");

        boost::optional<const ptree&> regions = configPt.get_child_optional("run.regions");

        // -----------------------------------------------------------------------------------------
        // Build population for each region
        // -----------------------------------------------------------------------------------------
        if (!regions) {
                pop->m_regions["Default"] = 0;
                pop->CreatePartitions(1);

                std::string geopop_type = configPt.get<std::string>("run.geopop_type", "default");

                CreateRegion(geopop_type, configPt, configPt.get_child("run"), pop, "Default", rnManager);
        } else {
                std::size_t currentId = 0;
                for (const auto& region : configPt.get_child("run.regions")) {
                        std::string name     = region.second.get<std::string>("name");
                        pop->m_regions[name] = currentId;
                        currentId++;
                }
                pop->CreatePartitions(currentId);

                for (const auto& region : configPt.get_child("run.regions")) {
                        std::string name        = region.second.get<std::string>("name");
                        std::string geopop_type = region.second.get<std::string>("geopop_type", "default");

                        CreateRegion(geopop_type, configPt, region.second, pop, name, rnManager);
                }
        }

        pop->Finalize();
        return pop;
}

std::shared_ptr<Population> Population::Create(const string& configString)
{
        return Create(RunConfigManager::FromString(configString));
}

std::shared_ptr<Population> Population::Create()
{
        // --------------------------------------------------------------
        // Create (empty) population and return it
        // --------------------------------------------------------------
        struct make_shared_enabler : public Population
        {
        };
        auto r                  = make_shared<make_shared_enabler>();
        r->m_regions["Default"] = 0;
        r->CreatePartitions(1);
        r->m_belief_pt.add("name", "NoBelief");
        return r;
}

unsigned int Population::GetAdoptedCount() const
{
        unsigned int total{0U};
        for (const auto& p : *this) {
                if (p.GetBelief()->HasAdopted()) {
                        total++;
                }
        }
        return total;
}

unsigned int Population::GetInfectedCount() const
{
        unsigned int total{0U};
        for (const auto& p : *this) {
                const auto& h = p.GetHealth();
                total += h.IsInfected() || h.IsRecovered();
        }
        return total;
}

void Population::CreatePerson(std::size_t regionId, unsigned int id, double age, unsigned int householdId,
                              unsigned int schoolId, unsigned int workId, unsigned int primaryCommunityId,
                              unsigned int secondaryCommunityId)
{
        if (m_lastRegionId != regionId) {
                assert(regionId > m_lastRegionId);
                /**
                 * From now on (since for now we don't use parallelism to generate the different regions) we will
                 * insert persons for the next region. To prevent the ContactPools id's to overlap we will calculate
                 * the previous max ContactPool's ids.
                 */
                m_previousRegionMaxId = m_currentRegionMaxId;
                m_lastRegionId        = regionId;
        }

        // Add the maximum ContactPool's id from the previous region
        householdId += m_previousRegionMaxId[ContactPoolType::Id::Household];
        schoolId += m_previousRegionMaxId[ContactPoolType::Id::School];
        workId += m_previousRegionMaxId[ContactPoolType::Id::Work];
        primaryCommunityId += m_previousRegionMaxId[ContactPoolType::Id::PrimaryCommunity];
        secondaryCommunityId += m_previousRegionMaxId[ContactPoolType::Id::SecondaryCommunity];

        m_currentRegionMaxId[ContactPoolType::Id::Household] =
            max(m_currentRegionMaxId[ContactPoolType::Id::Household], householdId);
        m_currentRegionMaxId[ContactPoolType::Id::School] =
            max(m_currentRegionMaxId[ContactPoolType::Id::School], schoolId);
        m_currentRegionMaxId[ContactPoolType::Id::Work] = max(m_currentRegionMaxId[ContactPoolType::Id::Work], workId);
        m_currentRegionMaxId[ContactPoolType::Id::PrimaryCommunity] =
            max(m_currentRegionMaxId[ContactPoolType::Id::PrimaryCommunity], primaryCommunityId);
        m_currentRegionMaxId[ContactPoolType::Id::SecondaryCommunity] =
            max(m_currentRegionMaxId[ContactPoolType::Id::SecondaryCommunity], secondaryCommunityId);

        emplace_back(regionId, id, age, householdId, schoolId, workId, primaryCommunityId, secondaryCommunityId);
}

void Population::Finalize()
{
        util::PartitionedSegmentedVector<Person>::Finalize();
        using namespace ContactPoolType;

        // --------------------------------------------------------------
        // Determine maximum pool ids in population.
        // --------------------------------------------------------------
        IdSubscriptArray<unsigned int> max_ids{0U};
        for (const auto& p : *this) {
                for (Id typ : IdList) {
                        max_ids[typ] = max(max_ids[typ], p.GetPoolId(typ));
                }
        }
        // --------------------------------------------------------------
        // Initialize poolSys with empty ContactPools (even for Id=0).
        // --------------------------------------------------------------
        for (Id typ : IdList) {
                for (size_t i = 0; i < max_ids[typ] + 1; i++) {
                        m_pool_sys[typ].emplace_back(ContactPool(i, typ));
                }
        }

        // --------------------------------------------------------------
        // Insert persons (pointers) in their contactpools. Having Id 0
        // means "not belonging pool of that type" (e.g. school/ work -
        // cannot belong to both, or e.g. out-of-work).
        //
        // Pools are uniquely identified by (typ, subscript) and a Person
        // belongs, for typ, to pool with subscrip p.GetPoolId(typ).
        // Defensive measure: we have a pool for Id 0 and leave it empty.
        // --------------------------------------------------------------
        for (auto& p : *this) {
                for (Id typ : IdList) {
                        const auto poolId = p.GetPoolId(typ);
                        if (poolId > 0) {
                                m_pool_sys[typ][poolId].AddMember(&p);
                        }
                }
        }
}

const std::unordered_map<std::string, std::size_t> Population::GetRegionIdentifiers() const { return m_regions; }
const util::SegmentedVector<Person>&               Population::GetRegion(const std::string& region) const
{
        return GetRegion(m_regions.at(region));
}
const util::SegmentedVector<Person>& Population::GetRegion(const std::size_t& region) const
{
        return GetPartition(region);
}

void Population::CreateRegion(const std::string& geopop_type, const boost::property_tree::ptree& configPt,
                              const boost::property_tree::ptree& regionPt, const std::shared_ptr<Population>& pop,
                              const std::string& name, stride::util::RNManager& rnManager)
{
        auto stride_logger = spdlog::get("stride_logger");

        if (geopop_type == "import") {
                if (stride_logger)
                        stride_logger->info("Creating region \"{}\" with imported pop.", name);
                ImportPopBuilder(configPt, regionPt, rnManager).Build(pop, pop->m_regions[name]);
        } else if (geopop_type == "generate") {
                if (stride_logger)
                        stride_logger->info("Creating region \"{}\" with generated pop.", name);
                GenPopBuilder(configPt, regionPt, rnManager).Build(pop, pop->m_regions[name]);
        } else {
                if (stride_logger)
                        stride_logger->info("Creating region \"{}\" with Default pop.", name);
                DefaultPopBuilder(configPt, regionPt, rnManager).Build(pop, pop->m_regions[name]);
        }
}
} // namespace stride
