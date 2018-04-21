#pragma once

#include <sim/event/Payload.h>
#include <spdlog/logger.h>
#include <visualizer/Visualizer.h>

namespace stride {
namespace viewers {
class VisualizerViewer
{
public:
        /// Instantiate cli viewer.
        VisualizerViewer(std::shared_ptr<spdlog::logger> logger) : m_logger(logger) {
                m_vis = std::make_unique<Visualizer>();
        }

        /// Let viewer perform update.
        void update(const sim_event::Payload& p);

private:
        std::shared_ptr<spdlog::logger> m_logger;
        bool m_first = true;
        std::unique_ptr<Visualizer> m_vis{};
};
} // namespace viewers
} // namespace stride
