#pragma once

#include "ContactPool.h"
#include <spdlog/logger.h>

#include <queue>

namespace stride {

class TravellerIndex
{
public:
        TravellerIndex();

        struct TravellerInfo
        {
                unsigned int        from;
                ContactPool*        to;
                Person*             person;
                std::size_t         leaveDay;
                ContactPoolType::Id type;
                bool                operator>(const TravellerInfo& other) const { return leaveDay > other.leaveDay; }
        };

        void StartTravel(unsigned int from, ContactPool* to, Person*, std::size_t leaveDay, ContactPoolType::Id type);

        void ReturnTravelers(std::size_t currentDay);

private:
        std::shared_ptr<spdlog::logger> m_travelLogger;

        std::priority_queue<TravellerInfo, std::vector<TravellerInfo>, std::greater<>> m_data;
};

} // namespace stride
