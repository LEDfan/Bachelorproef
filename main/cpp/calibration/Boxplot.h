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
        Boxplot(std::map<std::string, std::vector<unsigned int>> results);

        // Display the boxplots
        void Display();

        // Write the boxplots to files
        void WriteToFile();

private:
        double                                           FindMedian(int begin, int end, std::string testcase);
        void                                             GeneratePlots(bool write);
        std::map<std::string, std::vector<unsigned int>> results; ///< Storage for the simulation results
};

} // namespace calibration
