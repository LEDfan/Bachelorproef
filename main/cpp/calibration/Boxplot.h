#pragma once
#include <map>
#include <string>
#include <vector>

namespace calibration {

/**
 * A class used to generate QtCharts Boxplots
 * */
class Boxplot
{
public:
        // Setup
        Boxplot(std::map<std::string, std::vector<unsigned int>> results);

        // Display the boxplot
        void Display(int argc, char* argv[]);

private:
        double                                           FindMedian(int begin, int end, std::string testcase);
        std::map<std::string, std::vector<unsigned int>> results; ///< Storage for the simulation results
};

} // namespace calibration
