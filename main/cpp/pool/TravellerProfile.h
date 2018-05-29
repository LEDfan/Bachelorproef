#pragma once

#include <trng/discrete_dist.hpp>
#include <util/RNManager.h>
#include <vector>

namespace stride {

/**
 * Holds the information about travels
 */
class TravellerProfile
{
public:
        /// AmountOfTravel is a fraction 0-1 which indicates the change for a person to travel
        /// fractionWork indicates the amount of work travel
        TravellerProfile(std::size_t amountOfRegions, double amountOfTravel, double fractionWork,
                         util::RNManager& rnManager, unsigned int maxDays)
            : m_data_recreation(amountOfRegions), m_data_work(amountOfRegions), m_amountOfTravel(amountOfTravel),
              m_fractionWork(fractionWork), m_rnManager(rnManager), m_maxDays(maxDays){};

        explicit TravellerProfile(util::RNManager& rnManager)
            : m_data_recreation(), m_data_work(), m_amountOfTravel(), m_fractionWork(), m_rnManager(rnManager),
              m_maxDays(){};

        /// Add a travel record to the recreation profile
        void AddTravelRecreation(std::size_t from, std::size_t to, double relativePopulation);

        /// Add a travel record to the work profile
        void AddTravelWork(std::size_t from, std::size_t to, double relativePopulation);

        /// Based on the profile determines if a person will travel
        std::tuple<bool, bool, std::size_t, unsigned int> PersonWillTravel(std::size_t currentRegion);

private:
        /// From region, to region, relative amount of people
        std::vector<std::vector<double>> m_data_recreation;

        /// From region, to region, relative amount of people
        std::vector<std::vector<double>> m_data_work;

        /// A fraction 0-1 which indicates the change for a person to travel
        double m_amountOfTravel;

        /// A fraction 0-1 which indicates whether it's work travel or recreation
        double m_fractionWork;

        util::RNManager& m_rnManager;

        /// Maximum amount of days to travel
        unsigned int m_maxDays;

        bool MakeChoice(double fraction)
        {
                std::vector<double> weights;
                weights.push_back(1.0 - fraction); // -> 0, return is false -> not part of the fraction
                weights.push_back(fraction);       // -> 1, return is true -> part of the fraction

                auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));
                return static_cast<bool>(dist());
        }
};

} // namespace stride
