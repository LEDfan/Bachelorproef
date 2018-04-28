#include "Boxplot.h"
#include "Calibrator.h"
#include <tclap/CmdLine.h>

using namespace calibration;
using namespace TCLAP;

int main(int argc, char* argv[])
{
        CmdLine                cmd("calibration", ' ', "1.0");
        SwitchArg              display("d", "display", "Display boxplots on the screen", cmd);
        SwitchArg              write("w", "write", "Write boxplots to files in the current directory", cmd);
        ValueArg<unsigned int> count("c", "count", "The amount of simulations to run for each testcase", true, 10,
                                     "COUNT", cmd);
        cmd.parse(argc, static_cast<const char* const*>(argv));

        Calibrator calibrator;
        calibrator.Run(count.getValue());
        calibrator.PrintResults();
        auto    results = calibrator.GetResults();
        Boxplot boxplot(results);

        if (count.getValue() <= 1 && (display.getValue() || write.getValue())) {
                std::cerr << "Invalid parameters: cannot generate boxplots with count value " << count.getValue()
                          << std::endl;
                return 0;
        }

        if (display.getValue())
                boxplot.Display();
        else if (write.getValue())
                boxplot.WriteToFile();

        return 0;
}
