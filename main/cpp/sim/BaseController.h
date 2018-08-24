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

#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "sim/SimRunner.h"
#include "util/Stopwatch.h"

namespace stride {

/// A base class for Controllers in the MVC model
class BaseController
{
public:
        /// Straight initialization.
        explicit BaseController(std::string name, const boost::property_tree::ptree& configPt);

        /// Virtual desctructor for overloading
        virtual ~BaseController() = default;

        /// Actual run of the simulator.
        virtual void Control() = 0;

        /// Returns the SimRunner that is used
        virtual std::shared_ptr<SimRunner> GetSimRunner();

        /// Register a viewer of type T with given args
        template <typename T, typename... Targs>
        void RegisterViewer(Targs&&... args)
        {
                auto v = std::make_shared<T>(m_runner, std::forward<Targs>(args)...);
                m_runner->Register(v, bind(&T::Update, v, std::placeholders::_1));
        }

        /// Register the viewers of the SimRunner.
        virtual void RegisterViewers();

        /// Returns the logger
        virtual std::shared_ptr<spdlog::logger> GetLogger() const;

        /// Get the prefix path for output files
        virtual std::string GetOutputPrefix();

protected:
        /// Empty controller: used as taget for delegation.
        explicit BaseController();

        /// Check install environment.
        virtual void CheckEnv();

        // Output_prefix: if it's a string not containing any / it gets interpreted as a
        // filename prefix; otherwise we 'll create the corresponding directory.
        virtual void CheckOutputPrefix();

        /// Logs info on setup for cli environment to stride_logger.
        virtual void LogSetup();

        /// Make the appropriate logger for cli environment and register as stride_logger.
        void InstallLogger();

        /// Logs info on setup for cli environment to stride_logger.
        void LogShutdown();

        /// Logs info on setup for cli environment to stride_logger.
        void LogStartup();

        boost::property_tree::ptree     m_config_pt;        ///< Main configuration for run and sim.
        std::string                     m_output_prefix;    ///< Prefix to output (name prefix or prefix dir)
        util::Stopwatch<>               m_run_clock;        ///< Stopwatch for timing the computation.
        std::shared_ptr<spdlog::logger> m_stride_logger;    ///< General logger.
        bool                            m_use_install_dirs; ///< Working dir or install dir mode.
        std::shared_ptr<SimRunner>      m_runner;           ///< The SimRunner
        util::RnMan                     m_rn_manager;       ///< The manager for random numbers
        std::string                     m_name;             ///< Contoller's name.
};

} // namespace stride
