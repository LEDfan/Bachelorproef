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
 * Header for the command line controller.
 */

#include "sim/CliController.h"

#include "sim/SimRunner.h"
#include "util/ConfigInfo.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/TimeStamp.h"
#include "viewers/AdoptedViewer.h"
#include "viewers/CasesViewer.h"
#include "viewers/CliViewer.h"
#include "viewers/PersonsViewer.h"
#include "viewers/SummaryViewer.h"

#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using namespace stride::util;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace stride {

void CliController::CheckEnv()
{
        if (m_use_install_dirs) {
                auto log = [](const string& s)->void { std::cerr << s << std::endl; };
                if (!FileSys::CheckInstallEnv(log)) {
                        throw std::runtime_error("CliController::CheckEnv> Install dirs not OK.");
                }
        }
}

void CliController::CheckOpenMP()
{
        m_max_num_threads = ConfigInfo::NumberAvailableThreads();
}

void CliController::CheckOutputPrefix()
{
        if (FileSys::IsDirectoryString(m_output_prefix)) {
                try {
                        create_directories(m_output_prefix);
                } catch (std::exception &e) {
                        cerr << "CliController::Setup> Exception creating directory:  {}" << m_output_prefix << endl;
                        throw;
                }
        }
}


void CliController::Go()
{
        // -----------------------------------------------------------------------------------------
        // Instantiate SimRunner & register viewers & setup+execute the run.
        // -----------------------------------------------------------------------------------------
        // Necessary (i.o. local variable) because (quote) a precondition of shared_from_this(),
        // namely that at least one shared_ptr must already have been created (and still exist)
        // pointing to this. Shared_from_this is used in viewer notification mechanism.
        auto runner = make_shared<SimRunner>();

        // -----------------------------------------------------------------------------------------
        // Register viewers.
        // -----------------------------------------------------------------------------------------
        RegisterViewers(runner);

        // -----------------------------------------------------------------------------------------
        // Setup runner + execute the run.
        // -----------------------------------------------------------------------------------------
        runner->Setup(m_config_pt);
        runner->Run();
        m_logger->info("CliController shutting down. Timing: {}", m_run_clock.ToString());

        // -----------------------------------------------------------------------------------------
        // Done.
        // -----------------------------------------------------------------------------------------
        spdlog::drop_all();
}

void CliController::MakeLogger()
{
        const auto l = FileSys::BuildPath(m_output_prefix, "stride_log.txt");
        m_logger = m_silent_mode ? LogUtils::CreateNullLogger("stride_logger")
                                 : LogUtils::CreateCliLogger("stride_logger", l.string());
        spdlog::register_logger(m_logger);
}

void CliController::PatchConfig()
{
        // -----------------------------------------------------------------------------------------
        // Overrides/additions for configuration (using -p <param>=<value>) on commandline.
        // -----------------------------------------------------------------------------------------
        for (const auto& p : m_p_overrides) {
                m_config_pt.put("run." + get<0>(p), get<1>(p));
        }

        // -----------------------------------------------------------------------------------------
        // Config items that can ONLY specified with commandline options (-r, -s, -w NOT with -p)
        // -----------------------------------------------------------------------------------------
        m_config_pt.put("run.track_index_case", m_track_index_case);
        m_config_pt.put("run.silent_mode", m_silent_mode);
        m_config_pt.put("run.use_install_dirs", m_use_install_dirs);

        // -----------------------------------------------------------------------------------------
        // Num_threads.
        // -----------------------------------------------------------------------------------------
        const auto opt_num = m_config_pt.get_optional<unsigned int>("run.num_threads");
        if (!opt_num) {
                m_config_pt.put("run.num_threads", m_max_num_threads);
        }

        // -----------------------------------------------------------------------------------------
        // Output_prefix.
        // -----------------------------------------------------------------------------------------
        auto output_prefix = m_config_pt.get<string>("run.output_prefix", "");
        if (output_prefix.length() == 0) {
                output_prefix = TimeStamp().ToTag() + "/";
                m_config_pt.put("run.output_prefix", output_prefix);
        }
}

void CliController::ReadConfigFile()
{
        if (!exists(m_config_path) || !is_regular_file(m_config_path)) {
                cerr << "CliController::ReadConfigFile> Abort! File " << m_config_path.string()
                     << " not present." << endl;
                throw;
        } else {
                try {
                        read_xml(canonical(m_config_path).string(), m_config_pt, xml_parser::trim_whitespace);
                } catch (xml_parser_error& e) {
                        cerr << "CliController::ReadConfigFile> Abort! Error reading "
                             << m_config_path.string() << endl;
                        throw;
                }
        }
}

void CliController::RegisterViewers(shared_ptr<SimRunner> runner)
{
        // Command line viewer
        m_logger->info("Registering CliViewer");
        const auto cli_v = make_shared<viewers::CliViewer>(m_logger);
        runner->Register(cli_v, bind(&viewers::CliViewer::update, cli_v, placeholders::_1));

        // Adopted viewer
        if (m_config_pt.get<bool>("run.output_adopted", false)) {
                m_logger->info("registering AdoptedViewer,");
                const auto v = make_shared<viewers::AdoptedViewer>(m_output_prefix);
                runner->Register(v, bind(&viewers::AdoptedViewer::update, v, placeholders::_1));
        }

        // Cases viewer
        if (m_config_pt.get<bool>("run.output_cases", false)) {
                m_logger->info("Registering CasesViewer");
                const auto v = make_shared<viewers::CasesViewer>(m_output_prefix);
                runner->Register(v, bind(&viewers::CasesViewer::update, v, placeholders::_1));
        }

        // Persons viewer
        if (m_config_pt.get<bool>("run.output_persons", false)) {
                m_logger->info("registering PersonsViewer.");
                const auto v = make_shared<viewers::PersonsViewer>(m_output_prefix);
                runner->Register(v, bind(&viewers::PersonsViewer::update, v, placeholders::_1));
        }

        // Summary viewer
        if (m_config_pt.get<bool>("run.output_summary", false)) {
                m_logger->info("Registering SummaryViewer");
                const auto v = make_shared<viewers::SummaryViewer>(m_output_prefix);
                runner->Register(v, bind(&viewers::SummaryViewer::update, v, placeholders::_1));
        }
}

void CliController::Setup()
{
        // -----------------------------------------------------------------------------------------
        // Check environment.
        // -----------------------------------------------------------------------------------------
        CheckEnv();
        CheckOpenMP();

        // -----------------------------------------------------------------------------------------
        // Read config and patch where necessary.
        // -----------------------------------------------------------------------------------------
        m_config_path =
                (m_use_install_dirs) ? FileSys().GetConfigDir() /= m_config_file : system_complete(m_config_file);
        ReadConfigFile();
        PatchConfig();

        // -----------------------------------------------------------------------------------------
        // Deal with output_prefix.
        // -----------------------------------------------------------------------------------------
        m_output_prefix = m_config_pt.get<string>("run.output_prefix");
        CheckOutputPrefix();

        // -----------------------------------------------------------------------------------------
        // Create the appropriate logger and register it.
        // -----------------------------------------------------------------------------------------
        MakeLogger();

        // -----------------------------------------------------------------------------------------
        // Log the setup.
        // -----------------------------------------------------------------------------------------
        m_logger->info("CliController setup:");
        m_logger->info("Using configuration file:  {}", m_config_path.string());
        m_logger->info("Creating dir:  {}", m_output_prefix);
        m_logger->info("Executing:           {}", FileSys::GetExecPath().string());
        m_logger->info("Current directory:   {}", FileSys::GetCurrentDir().string());
        if (m_use_install_dirs) {
                m_logger->info("Install directory:   {}", FileSys::GetRootDir().string());
                m_logger->info("Config  directory:   {}", FileSys::GetConfigDir().string());
                m_logger->info("Data    directory:   {}", FileSys::GetDataDir().string());
        }
        if (ConfigInfo::HaveOpenMP()) {
                m_logger->info("Max number OpenMP threads in this environment: {}", m_max_num_threads);
                m_logger->info("Configured number of threads: {}", m_config_pt.get<unsigned int>("run.num_threads"));
        } else {
                m_logger->info("Not using OpenMP threads.");
        }
}

} // namespace stride