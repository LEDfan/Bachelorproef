#include "TestCalibrationRunner.h"
#include "../../test/cpp/gtester/ScenarioData.h"
#include "Boxplot.h"
#include "Calibrator.h"

namespace calibration {

TestCalibrationRunner::TestCalibrationRunner(std::vector<std::string> testcases) : CalibrationRunner()
{
        for (const auto& testcase : testcases) {
                const auto  d         = Tests::ScenarioData::Get(testcase);
                const auto& config_pt = std::get<0>(d);
                configs.push_back(std::make_pair(config_pt, testcase));
        }
}

} // namespace calibration
