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

        // ------------------------------------------------
        // Setup RNManager.
        // ------------------------------------------------
        RNManager rnManager(RNManager::Info{configPt.get<string>("pop.rng_type", "lcg64"),
                                            configPt.get<unsigned long>("run.rng_seed", 101UL), "",
                                            configPt.get<unsigned int>("run.num_threads")});

        // -----------------------------------------------------------------------------------------
        // Build population (at later date multiple builder or build instances ...).
        // -----------------------------------------------------------------------------------------
        std::string geopop_type = configPt.get<std::string>("run.geopop_type", "default");
        if (geopop_type == "import") {
                ImportPopBuilder(configPt, rnManager).Build(pop);
        } else if (geopop_type == "generate") {
                GenPopBuilder(configPt, rnManager).Build(pop);
        } else {
                DefaultPopBuilder(configPt, rnManager).Build(pop);
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
        auto r = make_shared<make_shared_enabler>();
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

void Population::CreatePerson(unsigned int id, double age, unsigned int householdId, unsigned int schoolId,
                              unsigned int workId, unsigned int primaryCommunityId, unsigned int secondaryCommunityId,
                              Health health, const ptree& beliefPt, double riskAverseness)
{
        string belief_policy = beliefPt.get<string>("name");

        if (belief_policy == "NoBelief") {
                NewPerson<NoBelief>(id, age, householdId, schoolId, workId, primaryCommunityId, secondaryCommunityId,
                                    health, beliefPt, riskAverseness);
        } else if (belief_policy == "Imitation") {
                NewPerson<Imitation>(id, age, householdId, schoolId, workId, primaryCommunityId, secondaryCommunityId,
                                     health, beliefPt, riskAverseness);
        } else {
                throw runtime_error(string(__func__) + "No valid belief policy!");
        }
}

void Population::CreatePerson(unsigned int id, double age, unsigned int household_id, unsigned int school_id,
                              unsigned int work_id, unsigned int primary_community_id,
                              unsigned int secondary_community_id, Health health, double risk_averseness)
{
        CreatePerson(id, age, household_id, school_id, work_id, primary_community_id, secondary_community_id, health,
                     m_belief_pt, risk_averseness);
}

template <typename BeliefPolicy>
void Population::NewPerson(unsigned int id, double age, unsigned int householdId, unsigned int schoolId,
                           unsigned int workId, unsigned int primaryCommunityId, unsigned int secondaryCommunityId,
                           Health health, const ptree& beliefPt, double riskAverseness)
{
        if (!m_beliefs_container) {
                m_beliefs_container.emplace<util::SegmentedVector<BeliefPolicy>>();
        }
        auto container = m_beliefs_container.cast<util::SegmentedVector<BeliefPolicy>>();

        assert(this->size() == container->size() && "Person and Beliefs container sizes not equal!");

        BeliefPolicy* bp = container->emplace_back(beliefPt);
        this->emplace_back(Person(id, age, householdId, schoolId, workId, primaryCommunityId, secondaryCommunityId,
                                  health, riskAverseness, bp));

        assert(this->size() == container->size() && "Person and Beliefs container sizes not equal!");
}

void Population::Finalize()
{
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

} // namespace stride
