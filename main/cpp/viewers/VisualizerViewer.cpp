#include "VisualizerViewer.h"
#include <iostream>

namespace stride {
namespace viewers {
void VisualizerViewer::update(const stride::sim_event::Payload& p)
{
    if(m_first){
        m_first = false;

        // Set the geogrid
//        m_vis->setGeoGrid(p.m_runner->GetSim());
    }

    // Update the markers
}
} // namespace viewers
} // namespace stride
