#include "MapViewer.h"
#include "sim/Sim.h"
#include "sim/SimRunner.h"
#include <iostream>

namespace stride {
namespace viewers {

void MapViewer::Update(const sim_event::Id /*id*/)
{
        m_logger->info("Visualizer updated");
        if (m_runner->GetSim() == nullptr) {
                return;
        }

        // CHeck if actually initialized
        while (!m_vis->IsReady()) {
                continue;
        }

        if (m_first) {
                m_first = false;

                // Set the geogrid
                m_vis->SetGeoGrid(m_runner->GetSim()->GetPopulation()->GetGeoGrid());
                m_vis->ForceUpdateMarkers();
                std::this_thread::sleep_for(std::chrono::seconds(5));
        } else {
                // Update the markers
                m_vis->ForceUpdateMarkers();
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
}

MapViewer::~MapViewer() { m_vis->Join(); }
} // namespace viewers
} // namespace stride
