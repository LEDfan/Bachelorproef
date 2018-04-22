#pragma once
#include <map>
#include <util/LogUtils.h>
#include <vector>

namespace calibration {

/**
 * A calibration tool used to calibrate the Stride scenario tests
 */
class Calibrator
{
public:
        // Setup the Calibrator
        Calibrator();

        // Run the actual simulations and gather results
        void Run();

        // Print the mean and standard deviation for the different testcases to the Logger
        void PrintResults();

        // Return the results of the simulations
        std::map<std::string, std::vector<unsigned int>> GetResults();

private:
        std::map<std::string, std::vector<unsigned int>> results; ///< Storage for the simulation results
        std::shared_ptr<spdlog::logger>                  logger;  ///< Logger to use for this class
};

} // namespace calibration
