#include "Calibrator.h"
#include "TestCalibrationRunner.h"
#include <tclap/CmdLine.h>

using namespace calibration;
using namespace TCLAP;

int main(int argc, char* argv[])
{
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
        cmd.parse(argc, static_cast<const char* const*>(argv));

        if (!count.isSet() && !single.isSet()) {
                std::cerr << "Please run at least one simulation." << std::endl;
                cmd.getOutput()->usage(cmd);
                return 1;
        }

        std::vector<std::string> tests;
        if (testcases.isSet()) {
                tests = testcases.getValue();
        } else {
                tests = {"influenza_a", "influenza_b", "influenza_c", "measles_16", "r0_12"};
        }

        TestCalibrationRunner testCalibrationRunner(tests);
        testCalibrationRunner.Run(count.getValue(), single.getValue());

        if (output.isSet())
                testCalibrationRunner.WriteResults(output.getValue());

        if (count.getValue() <= 1 && (write.getValue() || display.isSet())) {
                std::cerr << "Invalid parameters: cannot generate boxplots with count value " << count.getValue()
                          << std::endl;
        }
#ifdef QTCHARTS
        else if (display.isSet())
                testCalibrationRunner.DisplayBoxplots(display.getValue());
        else if (write.getValue())
                testCalibrationRunner.WriteBoxplots();
#else
        if (display.isSet() || write.getValue())
                std::cerr << "The calibration tool was not compiled with support for Qt5Charts" << std::endl;
#endif
        return 0;
}
