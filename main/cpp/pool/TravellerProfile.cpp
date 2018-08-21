#include "TravellerProfile.h"
#include "ContactPoolType.h"
#include <trng/uniform_int_dist.hpp>
#include <iostream>
#include <pop/Population.h>

namespace stride {

TravellerProfile::TravellerProfile(std::size_t amountOfRegions, double amountOfTravel, double fractionWork,
                                   util::RnMan& rnManager, std::size_t maxDays)
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
        if ((m_data_work.size() > 1 || m_data_recreation.size() > 1) && m_amountOfTravel > 0 &&
            MakeChoice(m_amountOfTravel)) {
                auto durationDist = m_rnManager[0].variate_generator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(m_maxDays)));
                std::size_t duration = static_cast<unsigned int>(durationDist());

                std::size_t leaveDay = currentDay + duration;

                std::function<trng::uniform_int_dist::result_type()> distRegion;

                bool tripIsWork = MakeChoice(m_fractionWork);

                ContactPoolType::Id type;
                if (m_data_work.empty() || !tripIsWork) {
                        type       = ContactPoolType::Id::PrimaryCommunity;
                        distRegion = m_rnManager[0].variate_generator(trng::uniform_int_dist(
                            0,
                            static_cast<trng::uniform_int_dist::result_type>(m_data_recreation[currentRegion].size())));
                } else {
                        type       = ContactPoolType::Id::Work;
                        distRegion = m_rnManager[0].variate_generator(trng::uniform_int_dist(
                            0, static_cast<trng::uniform_int_dist::result_type>(m_data_work[currentRegion].size())));
                }

                auto destinationRegion = static_cast<std::size_t>(distRegion());

                auto pools = population->SliceOnRegion(destinationRegion)[type];

                auto distLocation = m_rnManager[0].variate_generator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(pools.size())));

                ContactPool* destinationCp = &pools[distLocation()];

                population->GetTravellerIndex(destinationRegion)
                    .StartTravel(person->GetWorkId(), destinationCp, person, leaveDay, type);

                return true;
        }
        return false;
}

bool TravellerProfile::MakeChoice(double fraction)
{
        std::vector<double> weights;
        weights.push_back(1.0 - fraction); // -> 0, return is false -> not part of the fraction
        weights.push_back(fraction);       // -> 1, return is true -> part of the fraction

        auto dist = m_rnManager[0].variate_generator(trng::discrete_dist(weights.begin(), weights.end()));
        return static_cast<bool>(dist());
}

} // namespace stride
