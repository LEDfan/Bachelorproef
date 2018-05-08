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
class TestCalibrationRunner : public CalibrationRunner
{
public:
        /// Setup, retrieves the ptrees from the ScenarioData
        TestCalibrationRunner(std::vector<std::string> testcases);
};

} // namespace calibration
