#include "TravellerProfile.h"
#include "ContactPoolType.h"
#include <trng/uniform_int_dist.hpp>
#include <iostream>
#include <pop/Population.h>

namespace stride {

TravellerProfile::TravellerProfile(std::size_t amountOfRegions, double amountOfTravel, double fractionWork,
                                   util::RNManager& rnManager, unsigned int maxDays)
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

std::tuple<bool, bool, ContactPool*, unsigned int> TravellerProfile::PersonWillTravel(
    size_t currentRegion, std::shared_ptr<stride::Population> population)
{
        if (m_data_work.size() > 1 && m_data_recreation.size() > 1 && MakeChoice(m_amountOfTravel)) {
                std::size_t  destinationRegion = 0;
                ContactPool* destinationCp;
                bool         work;
                if (MakeChoice(m_fractionWork)) {
                        // Work travel
                        work = true;

                        auto distRegion = m_rnManager.GetGenerator(trng::uniform_int_dist(
                            0, static_cast<trng::uniform_int_dist::result_type>(m_data_work[currentRegion].size())));

                        destinationRegion = static_cast<size_t>(distRegion());

                        auto pools = population->SliceOnRegion(destinationRegion)[ContactPoolType::Id::Work];

                        auto distLocation = m_rnManager.GetGenerator(
                            trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(pools.size())));

                        destinationCp = &pools[distLocation()];

                } else {
                        // Recreation travel
                        work = false;

                        auto dist = m_rnManager.GetGenerator(trng::uniform_int_dist(
                            0,
                            static_cast<trng::uniform_int_dist::result_type>(m_data_recreation[currentRegion].size())));

                        destinationRegion = static_cast<size_t>(dist());

                        auto pools =
                            population->SliceOnRegion(destinationRegion)[ContactPoolType::Id::PrimaryCommunity];

                        auto distLocation = m_rnManager.GetGenerator(
                            trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(pools.size())));

                        destinationCp = &pools[distLocation()];
                }

                auto durationDist = m_rnManager.GetGenerator(trng::uniform_int_dist(0, m_maxDays));
                auto duration     = static_cast<unsigned int>(durationDist());

                return {true, work, destinationCp, duration};
        }
        return {false, false, 0, 0};
}

} // namespace stride
