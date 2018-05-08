#pragma once
#include <map>
#include <string>
#include <util/LogUtils.h>
#include <vector>

#include "BoxplotData.h"

namespace calibration {

// A class used to generate QtCharts Boxplots
class BoxplotGenerator
{
public:
        // Setup
        BoxplotGenerator();

        // Display the boxplots
        void Display(std::vector<BoxplotData> data) const;

        // Write the boxplots to files
        void WriteToFile(std::vector<BoxplotData> data) const;

private:
        void GeneratePlots(std::vector<BoxplotData> data,
                           bool write) const;   ///< Generate the actual plots. Write them to files if write is true,
                                                ///< otherwise display them on screen.
        std::shared_ptr<spdlog::logger> logger; ///< Logger to use for this class
};

} // namespace calibration
