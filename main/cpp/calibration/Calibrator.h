#pragma once
#include <map>
#include <util/LogUtils.h>
#include <vector>

namespace calibration {

class Calibrator
{
public:
        Calibrator();
        void run();
        void printResults();

private:
        std::map<std::string, std::vector<unsigned int>> results;
        std::shared_ptr<spdlog::logger>                  logger;
};

} // namespace calibration
