#include "TravellerProfile.h"
#include "ContactPoolType.h"
#include <trng/uniform_int_dist.hpp>
#include <iostream>
#include <pop/Population.h>

namespace stride {

TravellerProfile::TravellerProfile(std::size_t amountOfRegions, double amountOfTravel, double fractionWork,
                                   util::RNManager& rnManager, std::size_t maxDays)
    : m_data_recreation(amountOfRegions), m_data_work(amountOfRegions), m_amountOfTravel(amountOfTravel),
      m_fractionWork(fractionWork), m_rnManager(rnManager), m_maxDays(maxDays)
{
        for (std::size_t i = 0; i < amountOfRegions; ++i) {
                m_data_recreation[i] = std::vector<double>(amountOfRegions);
                m_data_work[i]       = std::vector<double>(amountOfRegions);
        }
}

void TravellerProfile::AddTravelRecreation(std::size_t from, std::size_t to, double relativePopulation)
{
        m_data_recreation.at(from).at(to) = relativePopulation;
}

void TravellerProfile::AddTravelWork(std::size_t from, std::size_t to, double relativePopulation)
{
        m_data_work.at(from).at(to) = relativePopulation;
}

bool TravellerProfile::PersonWillTravel(size_t currentRegion, std::shared_ptr<stride::Population> population,
                                        Person* person, std::size_t currentDay)
{
        if (m_data_work.size() > 1 && m_data_recreation.size() > 1 && m_amountOfTravel > 0 &&
            MakeChoice(m_amountOfTravel)) {
                auto durationDist = m_rnManager.GetGenerator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(m_maxDays)));
                std::size_t duration = static_cast<unsigned int>(durationDist());

                std::size_t leaveDay = currentDay + duration;

                auto distRegion = m_rnManager.GetGenerator(trng::uniform_int_dist(
                    0, static_cast<trng::uniform_int_dist::result_type>(m_data_work[currentRegion].size())));

                auto destinationRegion = static_cast<std::size_t>(distRegion());

                ContactPoolType::Id type;
                if (MakeChoice(m_fractionWork)) {
                        type = ContactPoolType::Id::Work;
                } else {
                        type = ContactPoolType::Id::PrimaryCommunity;
                }

                auto pools = population->SliceOnRegion(destinationRegion)[type];

                auto distLocation = m_rnManager.GetGenerator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(pools.size())));

                ContactPool* destinationCp = &pools[distLocation()];

                population->GetTravellerIndex(destinationRegion)
                    .StartTravel(person->GetWorkId(), destinationCp, person, leaveDay, ContactPoolType::Id::Work);

                return true;
        }
        return false;
}

} // namespace stride
