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
 * Main program: command line handling.
 */

#include "sim/CliController.h"
#include "util/StringUtils.h"

#include <tclap/CmdLine.h>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace stride;
using namespace TCLAP;

/// Main program of the stride simulator.
int main(int argc, char** argv)
{
        int exit_status = EXIT_SUCCESS;

        try {
                // -----------------------------------------------------------------------------------------
                // Parse command line.
                // -----------------------------------------------------------------------------------------
                CmdLine          cmd("stride", ' ', "1.0", false);
                SwitchArg        index_case_Arg("r", "r0", "R0 only", cmd, false);
                SwitchArg        silent_mode_Arg("s", "silent", "silent mode", cmd, false);
                ValueArg<string> config_file_Arg("c", "config", "Config File", false, "./config/run_default.xml",
                                                 "CONFIGURATION FILE", cmd);
                MultiArg<string> params_override_Arg("p", "param_override", "Config Parameter Override", false,
                                                     "parameter assignemnet", cmd);
                cmd.parse(argc, static_cast<const char* const*>(argv));

                // -----------------------------------------------------------------------------------------
                // Parse commandline config parameter overrides (if any).
                // -----------------------------------------------------------------------------------------
                vector<tuple<string, string>> p_overrides;
                const auto                    p_vec = params_override_Arg.getValue();
                for (const auto& p_assignment : p_vec) {
                        const auto v = util::Tokenize(p_assignment, "=");
                        p_overrides.push_back(make_tuple(v[0], v[1]));
                }
                // For now
                bool use_install_dirs = true;

                // -----------------------------------------------------------------------------------------
                // Run the Stride simulator.
                // -----------------------------------------------------------------------------------------
                CliController cntrl(index_case_Arg.getValue(), config_file_Arg.getValue(), p_overrides,
                                    silent_mode_Arg.getValue(), use_install_dirs);
                cntrl.Go();

        } catch (exception& e) {
                exit_status = EXIT_FAILURE;
                cerr << "\nEXCEPION THROWN: " << e.what() << endl;
        } catch (...) {
                exit_status = EXIT_FAILURE;
                cerr << "\nEXCEPION THROWN: Unknown exception." << endl;
        }
        return exit_status;
}
