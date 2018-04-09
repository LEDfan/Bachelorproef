#pragma once


#include "CliController.h"

namespace stride {
    class CliWithVisualizerController : public CliController {
    public:
        CliWithVisualizerController(bool track_index_case, std::string config_file,
                      std::vector<std::tuple<std::string, std::string>> p_overrides, bool silent_mode = false,
                      bool use_install_dirs = true) : CliController(track_index_case, config_file, p_overrides, silent_mode
        ){

            std::cout << "HALLO VAN DE VIS" << std::endl;
        };

    protected:
        /// Register the viewers of the SimRunner.
        void RegisterViewers(std::shared_ptr<SimRunner> runner, const std::string& output_prefix) override;


    };
}

