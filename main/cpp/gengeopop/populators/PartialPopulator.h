#pragma once

#include "../../util/RNManager.h"
#include <trng/discrete_dist.hpp>
#include <gengeopop/GeoGrid.h>
#include <gengeopop/GeoGridConfig.h>

namespace gengeopop {
/**
 * An interface for populators that provide a partial solution. They generate some data and apply it onto the GeoGrid.
 */
class PartialPopulator
{
public:
        PartialPopulator(stride::util::RNManager& rn_manager);
        virtual void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) = 0;
        virtual ~PartialPopulator(){};

protected:
        stride::util::RNManager& m_rnManager;

        template <typename T>
        std::vector<std::shared_ptr<ContactPool>> GetContactPoolInIncreasingRadius(
            const std::shared_ptr<GeoGrid>& geoGrid, const std::shared_ptr<Location>& start, double startRadius = 10)
        {
                double                                    currentRadius = startRadius;
                std::vector<std::shared_ptr<ContactPool>> pools;

                while (pools.empty()) {
                        for (const std::shared_ptr<Location>& nearLoc :
                             geoGrid->findLocationsInRadius(start, currentRadius)) {
                                const auto& centers = nearLoc->getContactCentersOfType<T>();
                                for (const auto& center : centers) {
                                        if (center->isAvailable()) {
                                                pools.insert(pools.end(), center->begin(), center->end());
                                        }
                                }
                        }
                        currentRadius *= 2;
                        if (currentRadius == std::numeric_limits<double>::infinity()) {
                                break;
                        }
                }
                return pools;
        }

        bool MakeChoice(double fraction)
        {
                std::vector<double> weights;
                weights.push_back(1.0 - fraction); // -> 0, return is false -> not part of the fraction
                weights.push_back(fraction);       // -> 1, return is true -> part of the fraction

                auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));
                return static_cast<bool>(dist());
        }
};
} // namespace gengeopop
