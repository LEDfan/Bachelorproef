#include "VisualizerViewer.h"
#include <iostream>
#include "sim/Simulator.h"
#include "sim/SimRunner.h"

namespace stride {
namespace viewers {
void VisualizerViewer::update(const stride::sim_event::Payload& p)
{
    m_logger->info("Visualizer updated");
    if(m_first){
//        m_first = false;

        // Set the geogrid
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if(p.m_runner->GetSim() == nullptr){
            std::cout << "no sim \U0001F985" << std::endl;
            return;
        }
        m_vis->setGeoGrid(p.m_runner->GetSim()->GetGeoGrid());
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    // Update the markers
}
} // namespace viewers
} // namespace stride
