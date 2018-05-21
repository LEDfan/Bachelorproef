#pragma once

#include <mapviewer/Map.h>
#include <sim/SimRunner.h>
#include <spdlog/logger.h>

namespace stride {
namespace viewers {
class MapViewer
{
public:
        /// Instantiate cli viewer.
        MapViewer(std::shared_ptr<SimRunner> runner, std::shared_ptr<spdlog::logger> logger,
                  std::shared_ptr<QQmlApplicationEngine> engine = nullptr)
            : m_logger(logger), m_vis{std::make_unique<Map>(engine)}, m_runner(std::move(runner))
        {
        }

        ~MapViewer();

        /// Let viewer perform update.
        void Update(const sim_event::Id id);

private:
        std::shared_ptr<spdlog::logger> m_logger; ///< The logger used for notifying that the MapViewer was updated
        bool m_first = true; ///< A variable used to see if it was the first time the "Update" method was called
        std::unique_ptr<Map>       m_vis;    ///< The Map to display
        std::shared_ptr<SimRunner> m_runner; ///< The runner for the current simulation
};
} // namespace viewers
} // namespace stride
