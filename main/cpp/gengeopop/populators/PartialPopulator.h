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
};
} // namespace gengeopop
