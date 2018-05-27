#pragma once
#include <map>
#include <string>
#include <util/LogUtils.h>
#include <vector>

#include "BoxplotData.h"

namespace calibration {

/**
 * A class used to compute the necessary information to generate boxplots
 */
class Boxplot
{
public:
        /// Setup
        Boxplot();

        /// Compute the information for each step in a list of testcase results
        std::vector<BoxplotData> Calculate(
            const std::map<std::string, std::vector<std::vector<unsigned int>>>& data) const;

        /// Compute the information for a specific step in a list of testcase results
        std::vector<BoxplotData> Calculate(const std::map<std::string, std::vector<std::vector<unsigned int>>>& data,
                                           unsigned int step) const;

        /// Compute the information for the last step in a list of testcase results
        std::vector<BoxplotData> CalculateLastStep(
            const std::map<std::string, std::vector<std::vector<unsigned int>>>& data) const;

        /// Compute the information for a single set of data
        BoxplotData Calculate(std::vector<unsigned int> data, std::string name) const;

private:
        /// Find the median of values between begin and end in the results. This vector needs to be sorted.
        double FindMedian(unsigned long begin, unsigned long end, std::vector<unsigned int>& results) const;

        std::shared_ptr<spdlog::logger> m_logger; ///< Logger to use for this class
};

} // namespace calibration
