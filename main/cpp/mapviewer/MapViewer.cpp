/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

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
