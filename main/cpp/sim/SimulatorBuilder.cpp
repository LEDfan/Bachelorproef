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
 * Implementation for the SimulatorBuilder class.
 */

#include "SimulatorBuilder.h"

#include "immunity/Vaccinator.h"
#include "pop/PopulationBuilder.h"
#include "util/FileSys.h"

#include "spdlog/sinks/null_sink.h"
#include <boost/property_tree/xml_parser.hpp>
#include <trng/uniform_int_dist.hpp>

namespace stride {

using namespace boost::property_tree;
using namespace std;
using namespace util;

SimulatorBuilder::SimulatorBuilder(const boost::property_tree::ptree& config_pt)
    : m_logger(nullptr), m_pt_config(config_pt)
{
        // If the execution context had created the stride_logger, use it.
        // Otherwise we produce a null logger so as not to have to guard all log statements ...
        m_logger = spdlog::get("stride_logger");
        if (!m_logger) {
                const auto null_sink = make_shared<spdlog::sinks::null_sink_st>();
                m_logger             = make_shared<spdlog::logger>("SimBuilder_null_logger", null_sink);
        }
}

std::shared_ptr<Simulator> SimulatorBuilder::Build()
{
        const auto pt_contact = ReadContactPtree();
        const auto pt_disease = ReadDiseasePtree();

        std::shared_ptr<Simulator> sim = nullptr;
        if (!pt_contact.empty() && !pt_disease.empty()) {
                sim = Build(pt_disease, pt_contact);
        }
        return sim;
}

ptree SimulatorBuilder::ReadContactPtree()
{
        const auto use_install_dirs = m_pt_config.get<bool>("run.use_install_dirs");

        ptree      pt;
        const auto fn = m_pt_config.get("run.age_contact_matrix_file", "contact_matrix.xml");
        const auto fp = (use_install_dirs) ? FileSys::GetDataDir() /= fn : fn;
        if (!exists(fp) || !is_regular_file(fp)) {
                m_logger->critical("Configuration file {} not present! Quitting.", fp.string());
        } else {
                m_logger->info("Configuration file:  {}", fp.string());
                try {
                        read_xml(canonical(fp).string(), pt, xml_parser::trim_whitespace);
                } catch (xml_parser_error& e) {
                        m_logger->critical("Error reading {}\nException: {}", canonical(fp).string(), e.what());
                        pt.clear();
                }
        }

        return pt;
}

ptree SimulatorBuilder::ReadDiseasePtree()
{
        const auto use_install_dirs = m_pt_config.get<bool>("run.use_install_dirs");

        ptree      pt;
        const auto fn = m_pt_config.get<string>("run.disease_config_file");
        const auto fp = (use_install_dirs) ? FileSys::GetDataDir() /= fn : fn;
        if (!exists(fp) || !is_regular_file(fp)) {
                m_logger->critical("Disease config file {} not present! Quitting.", fp.string());
        } else {
                m_logger->info("Disease config file:  {}", fp.string());
                try {
                        read_xml(canonical(fp).string(), pt, xml_parser::trim_whitespace);
                } catch (xml_parser_error& e) {
                        m_logger->critical("Error reading {}\nException: {}", canonical(fp).string(), e.what());
                        pt.clear();
                }
        }

        return pt;
}

std::shared_ptr<Simulator> SimulatorBuilder::Build(const ptree& pt_disease, const ptree& pt_contact)
{
        // --------------------------------------------------------------
        // Uninitialized simulator object.
        // --------------------------------------------------------------
        auto sim = make_shared<Simulator>();

        // --------------------------------------------------------------
        // Config info.
        // --------------------------------------------------------------
        sim->m_pt_config        = m_pt_config;
        sim->m_track_index_case = m_pt_config.get<bool>("run.track_index_case");
        sim->m_num_threads      = m_pt_config.get<unsigned int>("run.num_threads");
        sim->m_calendar         = make_shared<Calendar>(m_pt_config);

        // --------------------------------------------------------------
        // Initialize RNManager for random number engine management.
        // --------------------------------------------------------------
        const auto            rng_seed = m_pt_config.get<unsigned long>("run.rng_seed", 1UL);
        const auto            rng_type = m_pt_config.get<string>("run.rng_type", "mrg2");
        const RNManager::Info info{rng_type, rng_seed, "", sim->m_num_threads};
        sim->m_rn_manager.Initialize(info);

        // --------------------------------------------------------------
        // LogMode related initialization.
        // --------------------------------------------------------------
        const string l   = m_pt_config.get<string>("run.log_level", "None");
        sim->m_log_level = LogMode::IsLogMode(l)
                               ? LogMode::ToLogMode(l)
                               : throw runtime_error(string(__func__) + "> Invalid input for LogMode.");

        // --------------------------------------------------------------
        // Set correct information policies.
        // --------------------------------------------------------------
        const string loc_info_policy    = m_pt_config.get<string>("run.local_information_policy", "NoLocalInformation");
        sim->m_local_information_policy = loc_info_policy; // TODO make this enum class like LogMode

        // --------------------------------------------------------------
        // Build population.
        // --------------------------------------------------------------
        sim->m_population = PopulationBuilder::Build(m_pt_config, pt_disease, sim->m_rn_manager);

        // --------------------------------------------------------------
        // Contact profiles & initilize contactpools.
        // --------------------------------------------------------------
        using Id                                                 = ContactPoolType::Id;
        sim->m_contact_profiles[ToSizeT(Id::Household)]          = ContactProfile(Id::Household, pt_contact);
        sim->m_contact_profiles[ToSizeT(Id::School)]             = ContactProfile(Id::School, pt_contact);
        sim->m_contact_profiles[ToSizeT(Id::Work)]               = ContactProfile(Id::Work, pt_contact);
        sim->m_contact_profiles[ToSizeT(Id::PrimaryCommunity)]   = ContactProfile(Id::PrimaryCommunity, pt_contact);
        sim->m_contact_profiles[ToSizeT(Id::SecondaryCommunity)] = ContactProfile(Id::SecondaryCommunity, pt_contact);
        InitializeContactPools(sim);

        // --------------------------------------------------------------
        // Population immunity (natural immunity & vaccination).
        // --------------------------------------------------------------
        Vaccinator v(m_pt_config, sim->m_rn_manager);
        const auto immunity_profile = m_pt_config.get<std::string>("run.immunity_profile");
        v.Administer("immunity", immunity_profile, sim);
        const auto vaccination_profile = m_pt_config.get<std::string>("run.vaccine_profile");
        v.Administer("vaccine", vaccination_profile, sim);

        // --------------------------------------------------------------
        // Initialize disease profile.
        // --------------------------------------------------------------
        sim->m_operational = sim->m_disease_profile.Initialize(m_pt_config, pt_disease);

        // --------------------------------------------------------------
        // Seed infected persons.
        // --------------------------------------------------------------
        const auto seeding_rate         = m_pt_config.get<double>("run.seeding_rate");
        const auto seeding_age_min      = m_pt_config.get<double>("run.seeding_age_min", 1);
        const auto seeding_age_max      = m_pt_config.get<double>("run.seeding_age_max", 99);
        const auto pop_size             = sim->m_population->size() - 1;
        const auto max_population_index = static_cast<unsigned int>(pop_size);
        auto       int_generator = sim->m_rn_manager.GetGenerator(trng::uniform_int_dist(0, max_population_index));

        const auto contact_logger = spdlog::get("contact_logger");
        auto       num_infected   = static_cast<unsigned int>(floor(static_cast<double>(pop_size + 1) * seeding_rate));
        while (num_infected > 0) {
                Person& p = sim->m_population->at(static_cast<size_t>(int_generator()));
                if (p.GetHealth().IsSusceptible() && (p.GetAge() >= seeding_age_min) &&
                    (p.GetAge() <= seeding_age_max)) {
                        p.GetHealth().StartInfection();
                        num_infected--;
                        contact_logger->info("[PRIM] {} {} {} {}", -1, p.GetId(), -1, 0);
                }
        }

        // --------------------------------------------------------------
        // Done.
        // --------------------------------------------------------------
        return sim;
}

void SimulatorBuilder::InitializeContactPools(std::shared_ptr<Simulator> sim)
{
        // Determine the number of contactpools.
        unsigned int max_id_households{0U};
        unsigned int max_id_school_pools{0U};
        unsigned int max_id_work_pools{0U};
        unsigned int max_id_primary_community{0U};
        unsigned int max_id_secondary_community{0U};

        Population& population{*sim->m_population};
        using Id = ContactPoolType::Id;

        for (const auto& p : population) {
                max_id_households        = max(max_id_households, p.GetContactPoolId(Id::Household));
                max_id_school_pools      = max(max_id_school_pools, p.GetContactPoolId(Id::School));
                max_id_work_pools        = max(max_id_work_pools, p.GetContactPoolId(Id::Work));
                max_id_primary_community = max(max_id_primary_community, p.GetContactPoolId(Id::PrimaryCommunity));
                max_id_secondary_community =
                    max(max_id_secondary_community, p.GetContactPoolId(Id::SecondaryCommunity));
        }

        // Keep separate id counter to provide a unique id for every contactpool.
        unsigned int c_id = 1;

        for (size_t i = 0; i <= max_id_households; i++) {
                sim->m_households.emplace_back(ContactPool(c_id, Id::Household, sim->m_contact_profiles));
                c_id++;
        }
        for (size_t i = 0; i <= max_id_school_pools; i++) {
                sim->m_school_pools.emplace_back(ContactPool(c_id, Id::School, sim->m_contact_profiles));
                c_id++;
        }
        for (size_t i = 0; i <= max_id_work_pools; i++) {
                sim->m_work_pools.emplace_back(ContactPool(c_id, Id::Work, sim->m_contact_profiles));
                c_id++;
        }
        for (size_t i = 0; i <= max_id_primary_community; i++) {
                sim->m_primary_community.emplace_back(ContactPool(c_id, Id::PrimaryCommunity, sim->m_contact_profiles));
                c_id++;
        }
        for (size_t i = 0; i <= max_id_secondary_community; i++) {
                sim->m_secondary_community.emplace_back(
                    ContactPool(c_id, Id::SecondaryCommunity, sim->m_contact_profiles));
                c_id++;
        }

        // Having contactpool id '0' means "not present in any contactpool of that type".
        for (auto& p : population) {
                const auto hh_id = p.GetContactPoolId(Id::Household);
                if (hh_id > 0) {
                        sim->m_households[hh_id].AddMember(&p);
                }
                const auto sc_id = p.GetContactPoolId(Id::School);
                if (sc_id > 0) {
                        sim->m_school_pools[sc_id].AddMember(&p);
                }
                const auto wo_id = p.GetContactPoolId(Id::Work);
                if (wo_id > 0) {
                        sim->m_work_pools[wo_id].AddMember(&p);
                }
                const auto primCom_id = p.GetContactPoolId(Id::PrimaryCommunity);
                if (primCom_id > 0) {
                        sim->m_primary_community[primCom_id].AddMember(&p);
                }
                const auto secCom_id = p.GetContactPoolId(Id::SecondaryCommunity);
                if (secCom_id > 0) {
                        sim->m_secondary_community[secCom_id].AddMember(&p);
                }
        }
}

} // namespace stride
