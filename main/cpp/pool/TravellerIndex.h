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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#pragma once

#include "ContactPool.h"
#include "IdSubscriptArray.h"
#include <spdlog/logger.h>

#include <queue>

namespace stride {

/**
 * Stores the information about travels. One TravellerIndex stores information for a specific region. (i.e. all persons
 * stored are from the same region).
 * To have the code which handles the start and return of a person in the same location this class also handles that.
 */
class TravellerIndex
{
public:
        /// Default constructor
        TravellerIndex();

        /// Stores the information when the travel is started. F
        /// \p from is the original contactPool (type is \p type)
        /// \p to is the foreign contactPool (same type as from)
        /// \p leaveDay is the day at which the person will return
        /// \p type is the type of the travel Work or PrimaryCommunity (i.e. recreational)
        /// Should be thread safe (i.e. can be called from person while simulating)
        void StartTravel(unsigned int from, ContactPool* to, Person*, std::size_t leaveDay, ContactPoolType::Id type);

        /// Returns the travels at the given day
        /// Not guarenteed to be thread safe
        void ReturnTravelers(std::size_t currentDay);

private:
        /**
         * Information stored for a travel.
         */
        struct TravellerInfo
        {
                unsigned int                            from{};
                ContactPool*                            to{};
                Person*                                 person{};
                std::size_t                             leaveDay{};
                ContactPoolType::Id                     type{};
                ContactPoolType::IdSubscriptArray<bool> original_in_pools{};
                bool operator>(const TravellerInfo& other) const { return leaveDay > other.leaveDay; }
        };

        std::shared_ptr<spdlog::logger> m_travelLogger; ///< Logger to which the travel information will be logged

        /// min-heap to efficiently store the persons which are travelling. Indexed by the day they return (lowest
        /// first).
        std::priority_queue<TravellerInfo, std::vector<TravellerInfo>, std::greater<TravellerInfo>> m_data;
};

} // namespace stride
