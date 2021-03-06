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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

/**
 * @file
 * Initialize populations: implementation.
 */

#include "DefaultPopBuilder.h"

#include "pop/Population.h"
#include "pop/SurveySeeder.h"
#include "util/FileSys.h"
#include "util/RnMan.h"
#include "util/StringUtils.h"

#include <boost/property_tree/ptree.hpp>
#include <contact/ContactLogMode.h>
#include <fstream>
#include <pool/ContactPoolType.h>
#include <pool/IdSubscriptArray.h>

namespace stride {

using namespace ContactPoolType;

using namespace util;
using namespace boost::property_tree;

std::shared_ptr<Population> DefaultPopBuilder::MakePersons(std::shared_ptr<Population> pop)
{
        //------------------------------------------------
        // Read persons from file.
        //------------------------------------------------
        const auto file_name        = m_config_pt.get<std::string>("run.population_file");
        const auto use_install_dirs = m_config_pt.get<bool>("run.use_install_dirs");
        const auto file_path =
            (use_install_dirs) ? FileSys::GetDataDir() /= file_name : std::filesystem::path(file_name);
        if (!is_regular_file(file_path)) {
                throw std::runtime_error(std::string(__func__) + "> Population file " + file_path.string() +
                                         " not present.");
        }

        std::ifstream pop_file;
        pop_file.open(file_path.string());
        if (!pop_file.is_open()) {
                throw std::runtime_error(std::string(__func__) + "> Error opening population file " +
                                         file_path.string());
        }

        std::string line;
        getline(pop_file, line); // step over file header
        unsigned int person_id = 0U;

        while (getline(pop_file, line)) {
                const auto values                 = Split(line, ",");
                const auto age                    = FromString<unsigned int>(values[0]);
                const auto household_id           = FromString<unsigned int>(values[1]);
                const auto school_id              = FromString<unsigned int>(values[2]);
                const auto work_id                = FromString<unsigned int>(values[3]);
                const auto primary_community_id   = FromString<unsigned int>(values[4]);
                const auto secondary_community_id = FromString<unsigned int>(values[5]);

                pop->CreatePerson(m_regionId, person_id, age, household_id, school_id, 0, work_id, primary_community_id,
                                  secondary_community_id);
                ++person_id;
        }

        pop_file.close();
        return pop;
}

std::shared_ptr<Population> DefaultPopBuilder::Build(std::shared_ptr<Population> pop, std::size_t regionId, std::string)
{
        m_regionId = regionId;
        //------------------------------------------------
        // Check validity of input data.
        //------------------------------------------------
        const auto seeding_rate = m_config_pt.get<double>("run.seeding_rate");
        if (seeding_rate > 1.0) {
                throw std::runtime_error(std::string(__func__) + "> Bad input data for seeding_rate.");
        }

        //------------------------------------------------
        // Add persons & fill pools
        //------------------------------------------------
        SurveySeeder(m_config_pt, m_rn_manager).Seed(MakePersons(pop));

        // --------------------------------------------------------------
        // Determine maximum pool ids in population.
        // --------------------------------------------------------------
        IdSubscriptArray<unsigned int> max_ids{0U};
        for (const auto& p : *pop) {
                for (Id typ : IdList) {
                        max_ids[typ] = std::max(max_ids[typ], p.GetPoolId(typ));
                }
        }
        // --------------------------------------------------------------
        // Initialize poolSys with empty ContactPools (even for Id=0).
        // --------------------------------------------------------------
        for (Id typ : IdList) {
                for (size_t i = 0; i < max_ids[typ] + 1; i++) {
                        pop->m_pool_sys[typ].emplace_back(ContactPool(i, typ));
                }
        }

        // --------------------------------------------------------------
        // Insert persons (pointers) in their contactpools. Having Id 0
        // means "not belonging pool of that type" (e.g. school/ work -
        // cannot belong to both, or e.g. out-of-work).
        //
        // Pools are uniquely identified by (typ, subscript) and a Person
        // belongs, for typ, to pool with subscript p.GetPoolId(typ).
        // Defensive measure: we have a pool for Id 0 and leave it empty.
        // --------------------------------------------------------------
        for (auto& p : *pop) {
                for (Id typ : IdList) {
                        const auto poolId = p.GetPoolId(typ);
                        if (poolId > 0) {
                                pop->m_pool_sys[typ][poolId].AddMember(&p);
                        }
                }
        }
        return pop;
}

} // namespace stride
