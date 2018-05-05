#include "Boxplot.h"

#include <algorithm>

namespace calibration {

/*
 * Inspired by https://doc.qt.io/qt-5.10/qtcharts-boxplotchart-example.html
 * */

Boxplot::Boxplot() : logger(stride::util::LogUtils::CreateCliLogger("boxplot_logger", "boxplot_log.txt")) {}

double Boxplot::FindMedian(unsigned long begin, unsigned long end, std::vector<unsigned int> results) const
{
        std::sort(results.begin(), results.end());
        unsigned long count = end - begin;
        if (count % 2) {
                return results.at(count / 2 + begin);
        } else {
                double right = results.at(count / 2 + begin);
                double left  = results.at(count / 2 - 1 + begin);
                return (right + left) / 2.0;
        }
}
std::vector<BoxplotData> Boxplot::Calculate(const std::map<std::string, std::vector<std::vector<unsigned int>>>& data,
                                            unsigned int step) const
{
        std::vector<BoxplotData> boxplots;
        for (const auto& config : data) {
                std::vector<unsigned int> results;
                for (const auto& run : config.second) {
                        if (run.size() - 1 < step) {
                                logger->error("No such step {} for {} in input.", step, config.first);
                                continue;
                        }
                        results.push_back(run[step]);
                }
                boxplots.push_back(Calculate(results, config.first + " step " + std::to_string(step)));
        }
        return boxplots;
}
std::vector<BoxplotData> Boxplot::Calculate(
    const std::map<std::string, std::vector<std::vector<unsigned int>>>& data) const
{
        std::vector<BoxplotData> boxplots;
        for (const auto& config : data) {
                if (config.second.empty())
                        continue;
                for (unsigned int step = 0; step < config.second[0].size(); step++) {
                        std::vector<unsigned int> results;
                        for (const auto& run : config.second) {
                                if (run.size() - 1 < step) {
                                        logger->error(
                                            "Inconstistent input data! Not all runs have the same number of steps.");
                                        continue;
                                }
                                results.push_back(run[step]);
                        }
                        boxplots.push_back(Calculate(results, config.first + " step " + std::to_string(step)));
                }
        }
        return boxplots;
}

BoxplotData Boxplot::Calculate(const std::vector<unsigned int>& data, std::string name) const
{
        if (data.size() <= 1) {
                logger->warn("Warning: Could not generate information: not enough data.");
        }
        unsigned int count = data.size();
        BoxplotData  boxplotData;
        boxplotData.name           = name;
        boxplotData.min_value      = *std::min_element(data.begin(), data.end());
        boxplotData.max_value      = *std::max_element(data.begin(), data.end());
        boxplotData.median         = FindMedian(0, count, data);
        boxplotData.lower_quartile = FindMedian(0, static_cast<unsigned long>(count / 2.0), data);
        boxplotData.upper_quartile = FindMedian(static_cast<unsigned long>(count / 2.0 + (count % 2)), count, data);
        return boxplotData;
}

} // namespace calibration
