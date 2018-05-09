#include "Calibrator.h"
#include "ConfigFileCalibrationRunner.h"
#include "TestCalibrationRunner.h"
#include <tclap/CmdLine.h>

using namespace calibration;
using namespace TCLAP;

int main(int argc, char* argv[])
{
        // -----------------------------------------------------------------------------------------
        // Parse command line.
        // -----------------------------------------------------------------------------------------
        CmdLine                cmd("calibration", ' ', "1.0");
        ValueArg<unsigned int> display("d", "display", "Display the boxplots for a specified step", false, 0, "step",
                                       cmd);
        SwitchArg              write("w", "write", "Write boxplots to files in the current directory", cmd);
        ValueArg<std::string>  output("o", "output", "Write the results of the calibration to files", false,
                                     "results.txt", "filename", cmd);
        SwitchArg single("s", "single", "Run the simulations with the given seeds to determine the exact values", cmd);
        ValueArg<unsigned int> count("m", "multiple", "The amount of simulations to run for each testcase", false, 0,
                                     "amount", cmd);
        MultiArg<std::string> testcases("t", "testcases", "The testcases to use for the calibration", false, "testcase",
                                        cmd);
        std::string           sc = "Specifies the run configuration parameters to be used. It may be either "
                         "-c file=<file> or -c name=<name>. The first option can be shortened to -c <file>, the second "
                         "option accepts 'TestsInfluenza', 'TestsMeasles' or 'BenchMeasles' as <name>.";

        MultiArg<std::string> configArg("c", "config", sc, false, "RUN CONFIGURATION", cmd);
        cmd.parse(argc, static_cast<const char* const*>(argv));

        // -----------------------------------------------------------------------------------------
        // Validate provided options & setup calibrationRunner.
        // -----------------------------------------------------------------------------------------
        if (!count.isSet() && !single.isSet()) {
                std::cerr << "Please run at least one simulation." << std::endl;
                cmd.getOutput()->usage(cmd);
                return 1;
        }

        if (count.getValue() <= 1 && (write.getValue() || display.isSet())) {
                std::cerr << "Invalid parameters: cannot generate boxplots with count value " << count.getValue()
                          << std::endl;
                return 1;
        }

        std::shared_ptr<CalibrationRunner> calibrationRunner;
        if (configArg.isSet()) {
                if (testcases.isSet()) {
                        std::cerr << "Cannot run the calibration with both a configuration file and a testcase "
                                     "supplied, please choose one."
                                  << std::endl;
                        cmd.getOutput()->usage(cmd);
                        return 1;
                }
                calibrationRunner = std::make_shared<ConfigFileCalibrationRunner>(configArg.getValue());
        } else {
                std::vector<std::string> tests;
                if (testcases.isSet()) {
                        tests = testcases.getValue();
                } else {
                        tests = {"influenza_a", "influenza_b", "influenza_c", "measles_16", "r0_12"};
                }
                calibrationRunner = std::make_shared<TestCalibrationRunner>(tests);
        }

        // -----------------------------------------------------------------------------------------
        // Run simulations
        // -----------------------------------------------------------------------------------------
        calibrationRunner->Run(count.getValue(), single.getValue());

        // -----------------------------------------------------------------------------------------
        // Output.
        // -----------------------------------------------------------------------------------------
        if (output.isSet())
                calibrationRunner->WriteResults(output.getValue());

#if Qt5Charts_FOUND == true
        if (display.isSet())
                calibrationRunner->DisplayBoxplots(display.getValue());
        else if (write.getValue())
                calibrationRunner->WriteBoxplots();
#else
        if (display.isSet() || write.getValue())
                std::cerr << "The calibration tool was not compiled with support for Qt5Charts" << std::endl;
#endif
        return 0;
}
