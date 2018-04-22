#include "VisualizerViewer.h"
#include "sim/SimRunner.h"
#include "sim/Simulator.h"
#include <iostream>

namespace stride {
namespace viewers {
void VisualizerViewer::update(const stride::sim_event::Payload& p)
{
        m_logger->info("Visualizer updated");
        if (m_first and p.m_runner->GetSim() != nullptr) {
                m_first = false;

                // Set the geogrid
                m_vis->setGeoGrid(p.m_runner->GetSim()->GetGeoGrid());
                std::this_thread::sleep_for(std::chrono::seconds(5));
        } else {
                // Update the markers
                m_vis->forceUpdateMarkers();
                std::this_thread::sleep_for(std::chrono::seconds(5));
        }
}
} // namespace viewers
} // namespace stride
