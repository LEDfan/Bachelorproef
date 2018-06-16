#include "TravellerIndex.h"
#include <pop/Person.h>
#include <spdlog/formatter.h>
#include <util/LogUtils.h>

namespace stride {

TravellerIndex::TravellerIndex() : m_travelLogger(spdlog::get("travel_logger")), m_data() {}

void TravellerIndex::ReturnTravelers(std::size_t currentDay)
{
        m_travelLogger->info("Checking if travellers need to return on day {}", currentDay);

        while (!m_data.empty()) {
                const TravellerInfo& travellerInfo = m_data.top();
                assert(travellerInfo.leaveDay >= currentDay);
                if (travellerInfo.leaveDay > currentDay) {
                        break;
                }
                m_data.pop();
                Person* person = travellerInfo.person;

                // restore to original pool id
                person->m_pool_ids[travellerInfo.type] = travellerInfo.to->GetId();

                // remove person from foreign CP
                travellerInfo.to->RemoveMember(person);

                // m_in_pools should not be changed
                m_travelLogger->info(
                    "Person[{}] returns from travel, home_cp: {}, foreign_cp {}, type of travel was: {}",
                    person->GetId(), travellerInfo.from, travellerInfo.to->GetId(),
                    ContactPoolType::ToString(travellerInfo.type));
        }
}

void TravellerIndex::StartTravel(unsigned int from, stride::ContactPool* to, stride::Person* person,
                                 std::size_t leaveDay, ContactPoolType::Id type)
{
        assert(type == ContactPoolType::Id::Work || type == ContactPoolType::Id::PrimaryCommunity);

        TravellerInfo r{};
        // save the old id
        r.from     = from;
        r.to       = to;
        r.person   = person;
        r.leaveDay = leaveDay;
        r.type     = type;

        // update the new id
        person->m_pool_ids[type] = to->GetId();
        // add person to the foreign CP
        to->AddMember(person);

        // reset all in_pools except for the Work or PrimaryCommunity
        bool originInPool = person->m_in_pools[type];
        for (ContactPoolType::Id cpType : ContactPoolType::IdList) {
                person->m_in_pools[cpType] = false;
        }
        person->m_in_pools[type] = originInPool;

        m_travelLogger->info(
            "Person[{}] start travel, home_cp: {}, foreign_cp: {}, until day {}, type of travel is: {}",
            person->GetId(), from, to->GetId(), leaveDay, ContactPoolType::ToString(type));

        m_data.push(r);
}

} // namespace stride
