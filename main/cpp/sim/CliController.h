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
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

/**
 * @file
 * Header for the command line controller.
 */

#include "BaseController.h"
#include "util/Stopwatch.h"

#include <boost/property_tree/ptree_fwd.hpp>

namespace stride {

/**
 * Controls a simulation run initiated with the command line interface (cli).
 *
 * CliController setup functions include (@see ControlHelper):
 * \li checks the OpenMP environment
 * \li checks the file system environment
 * \li interprets and executes the ouput prefix
 * \li intalls a stride logger
 *
 * The CliController execution
 * \li creates a population (@see Population)
 * \li creates a simulation runner (@see SimRunner)
 * \li registers the appropriate viewers
 * \li runs the simulation
 */
class CliController : public BaseController
{
public:
        /// Straight initialization.
        explicit CliController(const boost::property_tree::ptree& configPt);

        /// Virtual desctructor for overloading
        ~CliController() override = default;

        /// Actual run of the simulator.
        void Control() override;
};

} // namespace stride
