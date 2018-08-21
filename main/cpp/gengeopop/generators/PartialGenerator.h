#pragma once

#include "../../util/RnMan.h"
#include <gengeopop/GeoGrid.h>
#include <gengeopop/GeoGridConfig.h>
#include <spdlog/logger.h>
#include <util/ExcAssert.h>

namespace gengeopop {
/**
 * An interface for generators that provide a partial solution. They generate some data and apply it onto the GeoGrid.
 */
class PartialGenerator
{
public:
        /// Constructor with a RnMan and a logger
        explicit PartialGenerator(stride::util::RnMan& rn_manager, std::shared_ptr<spdlog::logger> logger);

        /// Apply this PartialGenerator to the given geogrid, generating ContactCenters
        virtual void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) = 0;

        /// Virtual destructor for inheritance
        virtual ~PartialGenerator() = default;

protected:
        stride::util::RnMan&            m_rnManager; ///< RnManager used by generators
        std::shared_ptr<spdlog::logger> m_logger;    ///< Logger used by generators

        /// Make sure we're using a valid weight for random numbers
        void CheckWeight(const std::string& func, double weight)
        {
                ExcAssert(weight >= 0 && weight <= 1 && !std::isnan(weight),
                          "Invalid weight due to invalid input data in " + func +
                              ", weight: " + std::to_string(weight));
        }
};
} // namespace gengeopop
