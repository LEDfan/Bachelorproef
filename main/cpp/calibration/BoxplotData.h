#pragma once

#include <string>

namespace calibration {
/// The data that describes a boxplot
struct BoxplotData
{
        BoxplotData() : name(), min_value(), max_value(), median(), lower_quartile(), upper_quartile() {}

        std::string name;
        double      min_value;
        double      max_value;
        double      median;
        double      lower_quartile;
        double      upper_quartile;
};
} // namespace calibration
