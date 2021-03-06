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
 * Header for the command line controller.
 */

#include "sim/BaseController.h"

#include "pop/Population.h"
#include "sim/SimBuilder.h"
#include "sim/SimRunner.h"
#include "util/ConfigInfo.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/TimeStamp.h"
#include "viewers/AdoptedFileViewer.h"
#include "viewers/CliViewer.h"
#include "viewers/InfectedViewer.h"
#include "viewers/PersonsFileViewer.h"
#include "viewers/SummaryFileViewer.h"

#include <boost/property_tree/xml_parser.hpp>
#include <gengeopop/GeoGrid.h>

using namespace std;
using namespace stride::util;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace stride {

BaseController::BaseController()
    : m_config_pt(), m_output_prefix(""), m_run_clock("run"), m_stride_logger(nullptr), m_use_install_dirs(),
      m_runner(), m_rn_manager(), m_name()
{
        CheckEnv();
        CheckOutputPrefix();
        InstallLogger();
        LogSetup();

        m_rn_manager.Initialize(RnMan::Info{m_config_pt.get<string>("pop.rng_seed", "1,2,3,4"), "",
                                            m_config_pt.get<unsigned int>("run.num_threads")});

        auto pop = Population::Create(m_config_pt, m_rn_manager);
        m_runner = make_shared<SimRunner>(m_config_pt, pop, m_rn_manager);
}

BaseController::BaseController(std::string name, const ptree& configPt)
    : m_config_pt(configPt), m_output_prefix(""), m_run_clock("run"), m_stride_logger(nullptr), m_use_install_dirs(),
      m_runner(), m_rn_manager(), m_name(name)
{
        m_run_clock.Start();
        m_output_prefix    = m_config_pt.get<string>("run.output_prefix");
        m_use_install_dirs = m_config_pt.get<bool>("run.use_install_dirs");

        CheckEnv();
        CheckOutputPrefix();
        InstallLogger();
        LogSetup();

        m_rn_manager.Initialize(RnMan::Info{m_config_pt.get<string>("pop.rng_seed", "1,2,3,4"), "",
                                            m_config_pt.get<unsigned int>("run.num_threads")});

        auto pop = Population::Create(m_config_pt, m_rn_manager);
        m_runner = make_shared<SimRunner>(m_config_pt, pop, m_rn_manager);
}

void BaseController::CheckEnv()
{
        if (m_use_install_dirs) {
                auto log = [](const string& s) -> void { cerr << s << endl; };
                if (!FileSys::CheckInstallEnv(log)) {
                        throw runtime_error("BaseController::CheckEnv> Install dirs not OK.");
                }
        }
}

void BaseController::CheckOutputPrefix()
{
        if (FileSys::IsDirectoryString(m_output_prefix)) {
                if (!FileSys::CreateDirectory(m_output_prefix)) {
                        throw std::runtime_error("BaseController::Setup> Failed to create directory:  " +
                                                 m_output_prefix);
                }
        }
}

void BaseController::RegisterViewers()
{
        // Command line viewer
        m_stride_logger->info("Registering CliViewer");
        const auto cli_v = make_shared<viewers::CliViewer>(m_runner, m_stride_logger);
        m_runner->Register(cli_v, bind(&viewers::CliViewer::Update, cli_v, placeholders::_1));

        // Adopted viewer
        if (m_config_pt.get<bool>("run.output_adopted", false)) {
                m_stride_logger->info("registering AdoptedViewer,");
                RegisterViewer<viewers::AdoptedFileViewer>(m_output_prefix);
        }

        // Infection counts viewer
        if (m_config_pt.get<bool>("run.output_cases", false)) {
                m_stride_logger->info("Registering InfectedViewer");
                RegisterViewer<viewers::InfectedViewer>();
        }

        // Persons viewer
        if (m_config_pt.get<bool>("run.output_persons", false)) {
                m_stride_logger->info("registering PersonsViewer.");
                RegisterViewer<viewers::PersonsFileViewer>(m_output_prefix);
        }

        // Summary viewer
        if (m_config_pt.get<bool>("run.output_summary", false)) {
                m_stride_logger->info("Registering SummaryViewer");
                RegisterViewer<viewers::SummaryFileViewer>(m_output_prefix);
        }
}

void BaseController::LogSetup()
{
        m_stride_logger->info("{} stating up at: {}", m_name, TimeStamp().ToString());
        m_stride_logger->info("Executing revision {}", ConfigInfo::GitRevision());
        m_stride_logger->info("Creating dir:  {}", m_output_prefix);
        m_stride_logger->trace("Executing:           {}", FileSys::GetExecPath().string());
        m_stride_logger->trace("Current directory:   {}", FileSys::GetCurrentDir().string());
        if (m_use_install_dirs) {
                m_stride_logger->trace("Install directory:   {}", FileSys::GetRootDir().string());
                m_stride_logger->trace("Config  directory:   {}", FileSys::GetConfigDir().string());
                m_stride_logger->trace("Data    directory:   {}", FileSys::GetDataDir().string());
        }
        if (ConfigInfo::HaveOpenMP()) {
                m_stride_logger->info("Max number OpenMP threads in this environment: {}",
                                      ConfigInfo::NumberAvailableThreads());
                m_stride_logger->info("Configured number of threads: {}",
                                      m_config_pt.get<unsigned int>("run.num_threads"));
        } else {
                m_stride_logger->info("Not using OpenMP threads.");
        }
}

void BaseController::InstallLogger()
{
        const auto path     = FileSys::BuildPath(m_output_prefix, "stride_log.txt");
        const auto logLevel = m_config_pt.get<string>("run.stride_log_level");
        m_stride_logger     = LogUtils::CreateCliLogger("stride_logger", path.string());
        m_stride_logger->set_level(spdlog::level::from_str(logLevel));
        m_stride_logger->flush_on(spdlog::level::err);
        spdlog::register_logger(m_stride_logger);
}

void BaseController::LogShutdown()
{
        m_run_clock.Stop();
        m_stride_logger->info("{} shutting down after: {}", m_name, m_run_clock.ToString());
}

void BaseController::LogStartup()
{
        m_stride_logger->info("{} starting up at: {}", m_name, TimeStamp().ToString());
        m_stride_logger->info("Executing revision {}", ConfigInfo::GitRevision());
        m_stride_logger->info("Creating dir:  {}", m_output_prefix);
        m_stride_logger->trace("Executing:           {}", FileSys::GetExecPath().string());
        m_stride_logger->trace("Current directory:   {}", FileSys::GetCurrentDir().string());
        if (m_use_install_dirs) {
                m_stride_logger->trace("Install directory:   {}", FileSys::GetRootDir().string());
                m_stride_logger->trace("Config  directory:   {}", FileSys::GetConfigDir().string());
                m_stride_logger->trace("Data    directory:   {}", FileSys::GetDataDir().string());
        }
        if (ConfigInfo::HaveOpenMP()) {
                m_stride_logger->info("Max number OpenMP threads in this environment: {}",
                                      ConfigInfo::NumberAvailableThreads());
                m_stride_logger->info("Configured number of threads: {}",
                                      m_config_pt.get<unsigned int>("run.num_threads"));
        } else {
                m_stride_logger->info("Not using OpenMP threads.");
        }
}

std::shared_ptr<spdlog::logger> BaseController::GetLogger() const { return m_stride_logger; }

std::shared_ptr<SimRunner> BaseController::GetSimRunner() { return m_runner; }

std::string BaseController::GetOutputPrefix() { return m_output_prefix; }

} // namespace stride
