#include <viewers/VisualizerViewer.h>
#include "CliWithVisualizerController.h"
#include <sim/SimRunner.h>
#include <functional>

using namespace std;

namespace stride {

    void
    CliWithVisualizerController::RegisterViewers(std::shared_ptr<SimRunner> runner, const std::string &output_prefix) {
        CliController::RegisterViewers(runner, output_prefix);

        // Register the visualizer viewer
        getLogger()->info("Setting for output of summary: true");
        const auto v = make_shared<viewers::VisualizerViewer>(getLogger());
        runner->Register(v, bind(&viewers::VisualizerViewer::update, v, placeholders::_1));
    }
}

