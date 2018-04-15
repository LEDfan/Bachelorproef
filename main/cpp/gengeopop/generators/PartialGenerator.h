#pragma once

#include "../../util/RNManager.h"
#include <gengeopop/GeoGrid.h>
#include <gengeopop/GeoGridConfig.h>

namespace gengeopop {
/**
 * An interface for generators that provide a partial solution. They generate some data and apply it onto the GeoGrid.
 */
class PartialGenerator
{
public:
        explicit PartialGenerator(stride::util::RNManager& rn_manager);
        virtual void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) = 0;
        virtual ~PartialGenerator()                                                        = default;
        ;

protected:
        stride::util::RNManager& m_rnManager;
};
} // namespace gengeopop
