#include "Boxplot.h"
#include "Calibrator.h"

using namespace calibration;

int main(int argc, char* argv[])
{
        Calibrator calibrator;
        calibrator.Run();
        calibrator.PrintResults();
        auto results = calibrator.GetResults();

        Boxplot boxplot(results);
        boxplot.Display(argc, argv);
}
