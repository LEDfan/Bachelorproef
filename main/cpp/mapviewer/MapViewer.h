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

#pragma once

#include <mapviewer/Map.h>
#include <sim/SimRunner.h>
#include <spdlog/logger.h>

namespace stride {
namespace viewers {

/// A Viewer that visually shows the GeoGrids and associated information with Qt
class MapViewer
{
public:
        /// Instantiate cli viewer.
        MapViewer(std::shared_ptr<SimRunner> runner, std::shared_ptr<spdlog::logger> logger,
                  QQmlApplicationEngine* engine = nullptr)
            : m_logger(logger), m_vis{std::make_unique<Map>(engine)}, m_runner(std::move(runner))
        {
        }

        /// Destructor
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
