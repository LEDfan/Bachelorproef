#include "MapViewer.h"
#include "sim/Sim.h"
#include "sim/SimRunner.h"
#include <iostream>

namespace stride {
namespace viewers {

void MapViewer::Update(const sim_event::Id /*id*/)
{
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
                m_vis->SetGeoGrids(m_runner->GetSim()->GetPopulation()->GetGeoGrids());
                m_vis->ForceUpdateMarkers();
        } else {
                // Update the markers
                m_vis->ForceUpdateMarkers();
        }
        m_logger->info("MapViewer updated");
}

MapViewer::~MapViewer() {}
} // namespace viewers
} // namespace stride
