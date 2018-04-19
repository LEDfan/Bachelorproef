#pragma once

#include "CliController.h"

namespace stride {
class CliWithVisualizerController : public CliController
{
public:
        CliWithVisualizerController(std::string                                       config_file,
                                    std::vector<std::tuple<std::string, std::string>> p_overrides,
                                    bool track_index_case = false, std::string stride_log_level = "info",
                                    bool use_install_dirs = true)
            : CliController(config_file, p_overrides, track_index_case, stride_log_level, use_install_dirs)
        {

                std::cout << "HALLO VAN DE VIS" << std::endl;
        };

protected:
        /// Register the viewers of the SimRunner.
        void RegisterViewers(std::shared_ptr<SimRunner> runner) override;
};
} // namespace stride
