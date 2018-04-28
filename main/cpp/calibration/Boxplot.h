#pragma once
#include <map>
#include <string>
#include <vector>

namespace calibration {

/**
 * A class used to generate QtCharts Boxplots
 */
class Boxplot
{
public:
        // Setup
        explicit Boxplot(std::map<std::string, std::vector<unsigned int>>& results);

        // Display the boxplots
        void Display();

        // Write the boxplots to files
        void WriteToFile();

private:
        double FindMedian(
            unsigned long begin, unsigned long end,
            std::string testcase);      ///< Find the median of values between begin and end in the results[testcase]
        void GeneratePlots(bool write); ///< Generate the actual plots. Write them to files if write is true, otherwise
                                        ///< display them on screen.
        std::map<std::string, std::vector<unsigned int>> results; ///< Storage for the simulation results
};

} // namespace calibration
