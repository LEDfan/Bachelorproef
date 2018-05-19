#pragma once
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

#include "Calibrator.h"

namespace calibration {

/**
 * A runner for the Calibration
 */
class CalibrationRunner
{
public:
        /// Setup
        CalibrationRunner();

        /// Run the Calibrator
        void Run(unsigned int count, bool single);

        /// Write the results to their respective files (based on testcase name)
        void WriteResults(std::string filename);

        /// Write the boxplots to their respective files (based on testcase name)
        void WriteBoxplots();

        /// Display the generated boxplots for step 'step' in the simulation to the screen
        void DisplayBoxplots(unsigned int step);

protected:
        std::vector<std::pair<boost::property_tree::ptree, std::string>> configs; ///< The configs for the testcases
        std::map<std::string, std::vector<std::vector<unsigned int>>>    multipleResults; ///< Results from RunMultiple
        std::map<std::string, std::vector<unsigned int>>                 singleResults;   ///< Results from RunSingle
        Calibrator calibrator; ///< The Calibrator to use for calibrating the testcases
};

} // namespace calibration
