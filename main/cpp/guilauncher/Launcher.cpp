//
// Created by niels on 5/8/18.
//

#include <iostream>
#include <sim/BaseController.h>
#include <sim/CliController.h>
#include <util/FileSys.h>
#include <QtCore/QUrl>
#include <viewers/MapViewer.h>
#include <viewers/AdoptedViewer.h>
#include <viewers/CliViewer.h>
#include <viewers/InfectedViewer.h>
#include <viewers/PersonsViewer.h>
#include <viewers/SummaryViewer.h>
#include <util/TimeStamp.h>
#include "Launcher.h"

void Launcher::launch(bool showMapViewer, bool showAdoptedViewer, bool showCliViewer, bool showInfectedViewer, bool showPersonsViewer, bool showSummaryViewer) {
       int exitStatus = EXIT_SUCCESS;

//        try {
                // -----------------------------------------------------------------------------------------
                // Parse command line.
                // -----------------------------------------------------------------------------------------

//                string se = "Execute the indicated function. The clean function takes the configuration file "
//                            "specified by the --config file=<file> parameter, cleans it (indent, sort) and "
//                            "rewrites it to a new file. The dump function takes the built-in configuration "
//                            "(used for testing/benchmarking) specified by the --config name=<name> parameter "
//                            "and writes it, cleanly (i.e. indented, sorted) to a new file. The sim function "
//                            "runs the simulator using the configuration specified by the --config parameter."
//                            "The latter may use either --config file=<file> or --config name=<name>. The sim"
//                            "function is the default. The simgui function runs the simulator with a graphical"
//                            "interface. The geopop executes the geospatial synthetic population generator";
//                ValueArg<string> execArg("e", "exec", se, false, "sim", &vc, cmd);
//
//                string so = "Override configuration parameters in the configuration file. The format is "
//                            "--override <name>=<value>. It can be used multiple times.";
//                MultiArg<string> overrideArg("o", "override", so, false, "<NAME>=<VALUE>", cmd);
//
//                string sc = "Specifies the run configuration parameters to be used. It may be either "
//                            "-c file=<file> or -c name=<name>. The first is most commonly used and may be "
//                            "shortened to -c <file>.";
//                ValueArg<string> configArg("c", "config", sc, false, "run_default.xml", "RUN CONFIGURATION", cmd);
//
//                string si = "Look for configuration file specified by the -c file=<file> or -c <file> in the "
//                            "stride install directories";
//                SwitchArg installedArg("i", "installed", si, cmd, true);
//
//                SwitchArg show_visualiser("v", "visualizer", "Open a visualizer window when the simulation runs.", cmd,
//                                          false);
//

                // -----------------------------------------------------------------------------------------
                // Get configuration and path with overrides (if any).
                // -----------------------------------------------------------------------------------------
                boost::property_tree::ptree configPt;
                configPt = stride::util::FileSys::ReadPtreeFile(m_configPath);
                if (configPt.get<std::string>("run.output_prefix", "").empty()) {
                        configPt.put<std::string>("run.output_prefix", stride::util::TimeStamp().ToTag().append("/"));
                }
                configPt.sort();
                std::shared_ptr<stride::BaseController> controller = nullptr;

//                     Check if we need the visualiser
//                    if (m_showVisualizer) {
//                            controller = std::make_shared<stride::CliWithVisualizerController>(configPt);
//                    } else {
                            controller = std::make_shared<stride::CliController>(configPt);
//                    }

                    if(showMapViewer){
                        controller->RegisterViewer<stride::viewers::MapViewer>(controller->GetLogger());
                    }
                    if(showAdoptedViewer){
                        controller->RegisterViewer<stride::viewers::AdoptedViewer>(controller->GetOutputPrefix());
                    }
                    if(showCliViewer){
                        controller->RegisterViewer<stride::viewers::CliViewer>(controller->GetLogger());
                    }
                    if(showInfectedViewer){
                        controller->RegisterViewer<stride::viewers::InfectedViewer>(controller->GetOutputPrefix());
                    }
                    if(showPersonsViewer){
                        controller->RegisterViewer<stride::viewers::PersonsViewer>(controller->GetOutputPrefix());
                    }
                    if(showSummaryViewer){
                        controller->RegisterViewer<stride::viewers::SummaryViewer>(controller->GetOutputPrefix());
                    }
                    controller->Control();


//        } catch (std::exception& e) {
//                exitStatus = EXIT_FAILURE;
//                std::cerr << "\nEXCEPION THROWN: " << e.what() << std::endl;
//        } catch (...) {
//                exitStatus = EXIT_FAILURE;
//                std::cerr << "\nEXCEPION THROWN: Unknown exception." << std::endl;
//        }
        std::cout << "Exited with code: " << exitStatus << std::endl;
}

void Launcher::setConfigPath(QString file) {
    QUrl                                      info(file);
    m_configPath = info.toLocalFile().toStdString();

}
