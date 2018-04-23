#include "Boxplot.h"
#include "Calibrator.h"

using namespace calibration;

int main(int argc, char* argv[])
{
        Calibrator calibrator;
        // calibrator.Run();
        calibrator.PrintResults();
        auto results = calibrator.GetResults();

        for (unsigned int i = 0; i < 100; i++) {
                results["influenza_a"].push_back(i);
        }

        for (unsigned int i = 1000; i < 2000; i++) {
                results["influenza_b"].push_back(i);
        }

        Boxplot boxplot(results);
        // boxplot.Display();
        boxplot.WriteToFile();
}
