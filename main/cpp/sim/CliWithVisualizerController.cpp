#include "CliWithVisualizerController.h"
#include <functional>
#include <sim/SimRunner.h>
#include <viewers/VisualizerViewer.h>

using namespace std;

namespace stride {

void CliWithVisualizerController::RegisterViewers(std::shared_ptr<SimRunner> runner)
{
        CliController::RegisterViewers(runner);

        // Register the visualizer viewer
        getLogger()->info("Setting for output of summary: true");
        const auto v = make_shared<viewers::VisualizerViewer>(getLogger());
        runner->Register(v, bind(&viewers::VisualizerViewer::update, v, placeholders::_1));
}
} // namespace stride
