#include "TravellerProfile.h"
#include <trng/uniform_int_dist.hpp>
#include <iostream>

namespace stride {

void TravellerProfile::AddTravelRecreation(std::size_t from, std::size_t to, double relativePopulation)
{
        m_data_recreation.at(from) = {to, relativePopulation};
}

void TravellerProfile::AddTravelWork(std::size_t from, std::size_t to, double relativePopulation)
{
        m_data_work.at(from) = {to, relativePopulation};
}

std::tuple<bool, bool, std::size_t, unsigned int> TravellerProfile::PersonWillTravel(std::size_t currentRegion)
{
        if (m_data_work.size() > 1 && m_data_recreation.size() > 1 && MakeChoice(m_amountOfTravel)) {
                std::size_t destination = 0;
                bool        work;
                if (MakeChoice(m_fractionWork)) {
                        // Work travel
                        work = true;

                        auto dist = m_rnManager.GetGenerator(trng::uniform_int_dist(
                            0, static_cast<trng::uniform_int_dist::result_type>(m_data_work[currentRegion].size())));

                        destination = static_cast<size_t>(dist());
                } else {
                        // Recreation travel
                        work = false;

                        auto dist = m_rnManager.GetGenerator(trng::uniform_int_dist(
                            0,
                            static_cast<trng::uniform_int_dist::result_type>(m_data_recreation[currentRegion].size())));

                        destination = static_cast<size_t>(dist());
                }

                auto durationDist = m_rnManager.GetGenerator(trng::uniform_int_dist(0, m_maxDays));
                auto duration     = static_cast<unsigned int>(durationDist());

                return {true, work, destination, duration};
        }
        return {false, false, 0, 0};
}

} // namespace stride
