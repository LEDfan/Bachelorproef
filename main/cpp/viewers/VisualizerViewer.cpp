#include <iostream>
#include "VisualizerViewer.h"

namespace stride {
    namespace viewers {
        void stride::viewers::VisualizerViewer::update(const stride::sim_event::Payload &p) {
            std::cout << "Visualizer step" << std::endl;

        }
    }
}

