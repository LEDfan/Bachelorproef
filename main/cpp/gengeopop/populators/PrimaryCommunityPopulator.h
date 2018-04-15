#pragma once

#include "../GeoGridConfig.h"
#include "PartialPopulator.h"

namespace gengeopop {

class PrimaryCommunityPopulator : public PartialPopulator
{
public:
        explicit PrimaryCommunityPopulator(stride::util::RNManager& rn_manager);

        void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
