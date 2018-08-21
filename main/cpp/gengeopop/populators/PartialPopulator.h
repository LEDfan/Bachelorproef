#pragma once

#include "../../util/RnMan.h"
#include <trng/discrete_dist.hpp>
#include <gengeopop/GeoGrid.h>
#include <gengeopop/GeoGridConfig.h>
#include <spdlog/logger.h>

namespace gengeopop {
/**
 * An interface for populators that provide a partial solution. They generate some data and apply it onto the GeoGrid.
 */
class PartialPopulator
{
public:
        /// Construct with a RnMan and a logger
        PartialPopulator(stride::util::RnMan& rn_manager, std::shared_ptr<spdlog::logger> logger);

        /// Populate the given geogrid
        virtual void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) = 0;

        /// Virtual destructor for inheritance
        virtual ~PartialPopulator() = default;

protected:
        stride::util::RnMan&        m_rnManager; ///< RnManager used by populators
        std::shared_ptr<spdlog::logger> m_logger;    ///< Logger used by populators

        /// Find contactpools in `geoGrid` in an exponentially increasing radius, starting at `startRadius`, around
        /// `start` As soon as at least one pool is found, all pools within the current radius are returned May return
        /// an empty vector when there are no pools to be found
        template <typename T>
        std::vector<stride::ContactPool*> GetContactPoolInIncreasingRadius(const std::shared_ptr<GeoGrid>&  geoGrid,
                                                                           const std::shared_ptr<Location>& start,
                                                                           double startRadius = 10) const
        {
                double                            currentRadius = startRadius;
                std::vector<stride::ContactPool*> pools;

                while (pools.empty()) {
                        for (const std::shared_ptr<Location>& nearLoc :
                             geoGrid->FindLocationsInRadius(start, currentRadius)) {
                                const auto& centers = nearLoc->GetContactCentersOfType<T>();
                                for (const auto& center : centers) {
                                        pools.insert(pools.end(), center->begin(), center->end());
                                }
                        }
                        currentRadius *= 2;
                        if (currentRadius == std::numeric_limits<double>::infinity()) {
                                break;
                        }
                }
                return pools;
        }

        /// Convenience wrapper around m_rnManager
        bool MakeChoice(double fraction);
};
} // namespace gengeopop
