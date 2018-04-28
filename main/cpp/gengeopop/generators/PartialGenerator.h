#pragma once

#include "../../util/RNManager.h"
#include <gengeopop/GeoGrid.h>
#include <gengeopop/GeoGridConfig.h>
#include <spdlog/logger.h>
#include <util/exc_assert.h>

namespace gengeopop {
/**
 * An interface for generators that provide a partial solution. They generate some data and apply it onto the GeoGrid.
 */
class PartialGenerator
{
public:
        explicit PartialGenerator(stride::util::RNManager& rn_manager, std::shared_ptr<spdlog::logger> logger);
        virtual void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) = 0;
        virtual ~PartialGenerator(){};

protected:
        stride::util::RNManager&        m_rnManager; ///< RnManager used by generators
        std::shared_ptr<spdlog::logger> m_logger;    ///< Logger used by generators

        void checkWeight(const std::string& func, double weight)
        {
                exc_assert(weight >= 0 || weight <= 1 || !std::isnan(weight),
                           "Invalid weight due to invalid input data in " + func +
                               ", weight: " + std::to_string(weight));
        }
};
} // namespace gengeopop
