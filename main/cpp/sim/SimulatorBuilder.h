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
 * Header for the SimulatorBuilder class.
 */

#include <boost/property_tree/ptree.hpp>
#include <memory>

namespace spdlog {
class logger;
}

namespace stride {

class Simulator;

/**
 * Builds a simulator (@see Simulator) based a configuration property tree.
 * It
 * \li reads any additional configuration files (disease, contact, ...)
 * \li initializes calendar and random number manager for the simulator
 * \li builds a contact/transmission logger
 * \li builds a population (vector of persons)
 * \li initialize contactpools
 * \li deals with initial immunity and infection in the population
 * \li adds population members to their contact pools
 */
class SimulatorBuilder
{
public:
        /// Initializing SimulatorBuilder.
        SimulatorBuilder(const boost::property_tree::ptree& config_pt, std::shared_ptr<spdlog::logger> logger);

        /// Build the simulator.
        std::shared_ptr<Simulator> Build();

private:
        /// Build the simulator.
        std::shared_ptr<Simulator> Build(const boost::property_tree::ptree& pt_disease,
                                         const boost::property_tree::ptree& pt_contact);

        /// Initialize the contactpoolss.
        static void InitializeContactPools(std::shared_ptr<Simulator> sim);

        /// Get the contact configuration data.
        boost::property_tree::ptree ReadContactPtree();

        /// Get the disease configuration data.
        boost::property_tree::ptree ReadDiseasePtree();

private:
        boost::property_tree::ptree     m_config_pt;
        std::shared_ptr<spdlog::logger> m_logger;
};

} // namespace stride
