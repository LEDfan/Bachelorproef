#pragma once

#include "../../util/RNManager.h"
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
            const std::shared_ptr<GeoGrid>& geoGrid, const std::shared_ptr<Location>& start, double step = 10)
        {
                double                                    currentRadius = step;
                std::vector<std::shared_ptr<ContactPool>> pools;

                while (pools.empty()) {
                        for (const std::shared_ptr<Location>& nearLoc :
                             geoGrid->findLocationsInRadius(start, currentRadius)) {
                                auto centers = nearLoc->getContactCentersOfType<T>();
                                for (const auto& center : centers) {
                                        pools.insert(pools.end(), center->begin(), center->end());
                                }
                        }
                        currentRadius += step;
                }
                return pools;
        }
};
} // namespace gengeopop
