#include "TestCalibrationRunner.h"
#include "../../test/cpp/gtester/ScenarioData.h"
#include "Boxplot.h"
#include "Calibrator.h"

#ifdef QTCHARTS
#include "BoxplotGenerator.h"
#endif

namespace calibration {

TestCalibrationRunner::TestCalibrationRunner(std::vector<std::string> testcases)
    : configs(), multipleResults(), singleResults(), calibrator()
{
        for (const auto& testcase : testcases) {
                const auto  d         = Tests::ScenarioData::Get(testcase);
                const auto& config_pt = std::get<0>(d);
                configs.push_back(std::make_pair(config_pt, testcase));
        }
}

void TestCalibrationRunner::Run(unsigned int count, bool single)
{
        Calibrator calibrator;

        if (single) {
                singleResults = calibrator.RunSingle(configs);
                calibrator.PrintSingleResults(singleResults);
        }
        if (count > 0) {
                multipleResults = calibrator.RunMultiple(count, configs);
                calibrator.PrintMultipleResults(multipleResults);
        }
}

void TestCalibrationRunner::WriteResults(std::string filename)
{
        if (singleResults.size() && multipleResults.size())
                calibrator.WriteResults(singleResults, multipleResults, filename);
        else if (singleResults.size())
                calibrator.WriteSingleResults(singleResults, filename);
        else if (multipleResults.size())
                calibrator.WriteMultipleResults(multipleResults, filename);
}

#ifdef QTCHARTS

void TestCalibrationRunner::WriteBoxplots()
{
        Boxplot                  boxplot;
        std::vector<BoxplotData> data = boxplot.Calculate(multipleResults);
        BoxplotGenerator         boxplotGenerator;
        boxplotGenerator.WriteToFile(data);
}

void TestCalibrationRunner::DisplayBoxplots(unsigned int step)
{
        Boxplot                  boxplot;
        std::vector<BoxplotData> data = boxplot.Calculate(multipleResults, step);
        BoxplotGenerator         boxplotGenerator;
        boxplotGenerator.Display(data);
}

#endif

} // namespace calibration
