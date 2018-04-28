#include "CliWithVisualizerController.h"
#include <functional>
#include <sim/SimRunner.h>
#include <viewers/MapViewer.h>

using namespace std;

namespace stride {

void CliWithVisualizerController::RegisterViewers(std::shared_ptr<SimRunner> runner)
{
        CliController::RegisterViewers(runner);

        // Register the visualizer viewer
        GetLogger()->info("Setting for output of summary: true");
        const auto v = make_shared<viewers::MapViewer>(GetLogger());
        runner->Register(v, bind(&viewers::MapViewer::Update, v, placeholders::_1));
}
} // namespace stride
