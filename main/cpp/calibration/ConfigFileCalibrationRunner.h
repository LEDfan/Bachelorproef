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
        /// Setup
        ConfigFileCalibrationRunner(std::string configFile);
};

} // namespace calibration
