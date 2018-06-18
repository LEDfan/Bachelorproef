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
 * Main program: command line handling.
 */

#include "guicontroller/GuiController.h"
#include "mapviewer/MapViewer.h"
#include "sim/BaseController.h"
#include "sim/CliController.h"
#include "util/FileSys.h"
#include "util/RunConfigManager.h"
#include "util/StringUtils.h"
#include "util/TimeStamp.h"

#include <tclap/CmdLine.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace TCLAP;
using namespace boost::property_tree;

/// Main program of the stride simulator.
int main(int argc, char** argv)

{
        Q_INIT_RESOURCE(controllerqml);
        Q_INIT_RESOURCE(qml);
        int exitStatus = EXIT_SUCCESS;

        try {
                // -----------------------------------------------------------------------------------------
                // Parse command line.
                // -----------------------------------------------------------------------------------------
                CmdLine cmd("stride", ' ', "1.0");

                vector<string>           execs{"clean", "dump", "sim", "simgui", "geopop"};
                ValuesConstraint<string> vc(execs);
                string se = "Execute the indicated function. The clean function takes the configuration file "
                            "specified by the --config file=<file> parameter, cleans it (indent, sort) and "
                            "rewrites it to a new file. The dump function takes the built-in configuration "
                            "(used for testing/benchmarking) specified by the --config name=<name> parameter "
                            "and writes it, cleanly (i.e. indented, sorted) to a new file. The sim function "
                            "runs the simulator using the configuration specified by the --config parameter."
                            "The latter may use either --config file=<file> or --config name=<name>. The sim"
                            "function is the default. The simgui function runs the simulator with a graphical"
                            "interface. The geopop executes the geospatial synthetic population generator";
                ValueArg<string> execArg("e", "exec", se, false, "sim", &vc, cmd);

                string so = "Override configuration parameters in the configuration file. The format is "
                            "--override <name>=<value>. It can be used multiple times.";
                MultiArg<string> overrideArg("o", "override", so, false, "<NAME>=<VALUE>", cmd);

                string sc = "Specifies the run configuration parameters to be used. It may be either "
                            "-c file=<file> or -c name=<name>. The first is most commonly used and may be "
                            "shortened to -c <file>.";
                ValueArg<string> configArg("c", "config", sc, false, "run_default.xml", "RUN CONFIGURATION", cmd);

                string si = "Look for configuration file specified by the -c file=<file> or -c <file> in the "
                            "stride install directories";
                SwitchArg installedArg("i", "installed", si, cmd, true);

                SwitchArg show_mapviewer("v", "mapviewer", "Open a mapviewer window when the simulation runs.", cmd,
                                         false);

                cmd.parse(argc, static_cast<const char* const*>(argv));

                // -----------------------------------------------------------------------------------------
                // Get configuration and path with overrides (if any).
                // -----------------------------------------------------------------------------------------
                auto  config = configArg.getValue();
                ptree configPt;

                if (regex_search(config, regex("^name="))) {
                        config   = regex_replace(config, regex(string("^name=")), string(""));
                        configPt = RunConfigManager::Create(config);
                } else {
                        config = regex_replace(config, regex(string("^file=")), string(""));
                        const boost::filesystem::path configPath =
                            (installedArg.getValue()) ? FileSys::GetConfigDir() /= config : config;
                        configPt = FileSys::ReadPtreeFile(configPath);
                }

                for (const auto& p_assignment : overrideArg.getValue()) {
                        const auto v = util::Tokenize(p_assignment, "=");
                        configPt.put("run." + v[0], v[1]);
                }

                // -----------------------------------------------------------------------------------------
                // If run simulation in cli or gui ...
                // -----------------------------------------------------------------------------------------
                if (execArg.getValue() == "sim" || execArg.getValue() == "simgui") {
                        if (configPt.get<string>("run.output_prefix", "").empty()) {
                                configPt.put("run.output_prefix", TimeStamp().ToTag().append("/"));
                        }
                        configPt.sort();

                        std::unique_ptr<BaseController> controller = nullptr;
                        QQmlApplicationEngine*          engine     = nullptr;

                        if (execArg.getValue() == "sim") {
                                controller = std::make_unique<CliController>(configPt);
                        } else {
                                auto temp  = std::make_unique<GuiController>(configPt);
                                engine     = temp->GetEngine();
                                controller = std::move(temp);
                        }
                        controller->RegisterViewers();

                        std::unique_ptr<std::thread> thread = nullptr;

                        if (show_mapviewer.getValue()) {
#if Qt5_FOUND
                                if (!engine) {
                                        Q_INIT_RESOURCE(qml);
                                        int             i = 0;
                                        QGuiApplication app(i, nullptr);
                                        auto            localEngine = std::make_unique<QQmlApplicationEngine>();
                                        controller->RegisterViewer<viewers::MapViewer>(controller->GetLogger(),
                                                                                       localEngine.get());
                                        thread =
                                            std::make_unique<std::thread>([&controller]() { controller->Control(); });
                                        app.exec();
                                } else {
                                        controller->RegisterViewer<viewers::MapViewer>(controller->GetLogger(), engine);
                                }
#else
                                std::cerr << "Can't run with mapviewer when Qt is not found" << std::endl;
#endif
                        }
                        if (thread) {
                                thread->join();
                        } else {
                                controller->Control();
                        }
                }
                // -----------------------------------------------------------------------------------------
                // If geopop ...
                // -----------------------------------------------------------------------------------------
                else if (execArg.getValue() == "geopop") {
                        cout << "Not implented here yet ..." << endl; // TODO
                }
                // -----------------------------------------------------------------------------------------
                // If clean/dump ...
                // -----------------------------------------------------------------------------------------
                else if (execArg.getValue() == "clean" || execArg.getValue() == "dump") {
                        RunConfigManager::CleanConfigFile(configPt);
                }
        } catch (exception& e) {
                exitStatus = EXIT_FAILURE;
                cerr << "\nEXCEPION THROWN: " << e.what() << endl;
        } catch (...) {
                exitStatus = EXIT_FAILURE;
                cerr << "\nEXCEPION THROWN: Unknown exception." << endl;
        }
        return exitStatus;
}
