#pragma once

#include <sim/SimRunner.h>
#include <spdlog/logger.h>
#include <visualizer/Visualizer.h>

namespace stride {
namespace viewers {
class MapViewer
{
public:
        /// Instantiate cli viewer.
        MapViewer(std::shared_ptr<SimRunner> runner, std::shared_ptr<spdlog::logger> logger,
                  std::shared_ptr<QQmlApplicationEngine> engine = nullptr)
            : m_logger(logger), m_vis{std::make_unique<Visualizer>(engine)}, m_runner(std::move(runner))
        {
        }

        ~MapViewer();

        /// Let viewer perform update.
        void Update(const sim_event::Id id);

private:
        std::shared_ptr<spdlog::logger> m_logger;
        bool                            m_first = true;
        std::unique_ptr<Visualizer>     m_vis;
        std::shared_ptr<SimRunner>      m_runner;
};
} // namespace viewers
} // namespace stride
