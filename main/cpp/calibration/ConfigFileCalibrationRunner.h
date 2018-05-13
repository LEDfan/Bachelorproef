#pragma once
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

#include "CalibrationRunner.h"
#include "Calibrator.h"

namespace calibration {

/**
 * A runner for the Calibration
 */
class ConfigFileCalibrationRunner : public CalibrationRunner
{
public:
        /// Setup, creates a ptree based on the filenames in configFiles
        ConfigFileCalibrationRunner(std::vector<std::string> configFiles);
};

} // namespace calibration
