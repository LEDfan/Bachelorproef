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
        if ((m_data_work.size() > 0 || m_data_recreation.size() > 0) && m_amountOfTravel > 0 &&
            MakeChoice(m_amountOfTravel)) {
                auto durationDist = m_rnManager[0].variate_generator(
                    trng::uniform_int_dist(0, static_cast<trng::uniform_int_dist::result_type>(m_maxDays)));
                std::size_t duration = static_cast<unsigned int>(durationDist());

                std::size_t leaveDay = currentDay + duration;

                std::function<trng::uniform_int_dist::result_type()> distRegion;
                std::vector<double>                                  weights;

                bool tripIsWork = MakeChoice(m_fractionWork);

                ContactPoolType::Id type;
                if (m_data_work.empty() || !tripIsWork) {
                        type    = ContactPoolType::Id::PrimaryCommunity;
                        weights = m_data_recreation[currentRegion];
                } else {
                        type    = ContactPoolType::Id::Work;
                        weights = m_data_work[currentRegion];
                }
                double sum = std::accumulate(weights.begin(), weights.end(), 0.0);
                if (!sum)
                        return false; // No weight information available for this origin region -> don't travel from
                                      // this region and this type of trip
                if (std::abs(sum - 1) > 0.001)
                        throw std::runtime_error("Invalid travel information, the sum of relative values has to be "
                                                 "equal to 1, instead it is " +
                                                 std::to_string(sum) + ".");

                distRegion = m_rnManager[0].variate_generator(trng::discrete_dist(weights.begin(), weights.end()));
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
