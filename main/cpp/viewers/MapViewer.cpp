#include "MapViewer.h"
#include "sim/SimRunner.h"
#include "sim/Simulator.h"
#include <iostream>

namespace stride {
namespace viewers {
void MapViewer::Update(const stride::sim_event::Payload& p)
{
        m_logger->info("Visualizer updated");
        if(p.m_runner->GetSim() == nullptr ){
                return;
        }

        if (m_first) {
                m_first = false;

                // Set the geogrid
                m_vis->SetGeoGrid(p.m_runner->GetSim()->GetGeoGrid());
                std::this_thread::sleep_for(std::chrono::seconds(5));
        } else {
                // Update the markers
                m_vis->ForceUpdateMarkers();
                std::this_thread::sleep_for(std::chrono::seconds(10));
        }
}
} // namespace viewers
} // namespace stride
