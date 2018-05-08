#include "ConfigFileCalibrationRunner.h"
#include "../../test/cpp/gtester/ScenarioData.h"
#include "Boxplot.h"
#include "Calibrator.h"
#include "util/FileSys.h"
#include "util/RunConfigManager.h"

#include <regex>

namespace calibration {

ConfigFileCalibrationRunner::ConfigFileCalibrationRunner(std::vector<std::string> configFiles) : CalibrationRunner()
{
        for (auto config : configFiles) {
                boost::property_tree::ptree configPt;

                if (regex_search(config, std::regex("^name="))) {
                        config   = regex_replace(config, std::regex(std::string("^name=")), std::string(""));
                        configPt = stride::util::RunConfigManager::Create(config);
                } else {
                        config = regex_replace(config, std::regex(std::string("^file=")), std::string(""));
                        const boost::filesystem::path configPath = config;
                        configPt                                 = stride::util::FileSys::ReadPtreeFile(configPath);
                }
                configs.push_back(std::make_pair(configPt, config));
        }
}

} // namespace calibration
