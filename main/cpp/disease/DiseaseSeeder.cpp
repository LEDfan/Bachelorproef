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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Implementation for the DiseaseSeeder class.
 */

#include "DiseaseSeeder.h"

#include "disease/Immunizer.h"
#include "pool/ContactPoolSys.h"
#include "pop/Population.h"
#include "sim/Sim.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/StringUtils.h"

#include <trng/uniform_int_dist.hpp>
#include <cassert>

namespace stride {

using namespace boost::property_tree;
using namespace stride::ContactPoolType;
using namespace util;
using namespace std;

DiseaseSeeder::DiseaseSeeder(const ptree& configPt, RNManager& rnManager)
    : m_config_pt(configPt), m_rn_manager(rnManager)
{
}

void DiseaseSeeder::Seed(std::shared_ptr<Population> pop)
{
        boost::optional<const ptree&> regionsToSeed = m_config_pt.get_child_optional("run.regions_to_seed");
        // --------------------------------------------------------------
        // Population immunity (natural immunity & vaccination).
        // --------------------------------------------------------------
        const auto immunityProfile = m_config_pt.get<std::string>("run.immunity_profile");
        Vaccinate("immunity", immunityProfile, pop->GetContactPoolSys()[Id::Household]);

        const auto vaccinationProfile = m_config_pt.get<std::string>("run.vaccine_profile");
        Vaccinate("vaccine", vaccinationProfile, pop->GetContactPoolSys()[Id::Household]);

        if (regionsToSeed) {
                for (const auto& region : m_config_pt.get_child("run.regions_to_seed")) {
                        std::cout << "Seeding region" << region.second.data() << std::endl;
                        std::size_t regionId = pop->GetRegionIdentifiers().at(region.second.data());

                        auto cps = pop->SliceOnRegion(regionId)[Id::Household];
                        SeedPop(pop->GetPersonInRegion(regionId), pop->GetContactLogger());
                }
        } else {
                SeedPop(*pop, pop->GetContactLogger());
        }
}
template <typename T>
void DiseaseSeeder::SeedPop(T& pop, std::shared_ptr<spdlog::logger> contactLogger)
{
        // --------------------------------------------------------------
        // Seed infected persons.
        // --------------------------------------------------------------
        const auto sRate   = m_config_pt.get<double>("run.seeding_rate");
        const auto sAgeMin = m_config_pt.get<double>("run.seeding_age_min", 1);
        const auto sAgeMax = m_config_pt.get<double>("run.seeding_age_max", 99);

        const auto popSize     = pop.size();
        const auto maxPopIndex = static_cast<unsigned int>(popSize - 1);
        auto       generator   = m_rn_manager.GetGenerator(trng::uniform_int_dist(0, maxPopIndex));
        //        auto&      logger      = pop.GetContactLogger();

        auto numInfected = static_cast<unsigned int>(floor(static_cast<double>(popSize) * sRate));
        while (numInfected > 0) {
                Person& p = pop[static_cast<size_t>(generator())];
                if (p.GetHealth().IsSusceptible() && (p.GetAge() >= sAgeMin) && (p.GetAge() <= sAgeMax)) {
                        p.GetHealth().StartInfection();
                        numInfected--;
                        contactLogger->info("[PRIM] {} {} {} {}", -1, p.GetId(), -1, 0);
                }
        }
}

template <typename T>
void DiseaseSeeder::Vaccinate(const std::string& immunityType, const std::string& immunizationProfile, T& immunityPools)
{
        std::vector<double> immunityDistribution;
        const double        linkProbability = 0;
        Immunizer           immunizer(m_rn_manager);

        if (immunizationProfile == "Random") {
                const auto immunityRate = m_config_pt.get<double>("run." + ToLower(immunityType) + "_rate");
                for (unsigned int index_age = 0; index_age < 100; index_age++) {
                        immunityDistribution.push_back(immunityRate);
                }
                immunizer.Random(immunityPools, immunityDistribution, linkProbability);
        } else if (immunizationProfile == "Cocoon") {
                immunizer.Cocoon(immunityPools, immunityDistribution, linkProbability);
        }
}

} // namespace stride
