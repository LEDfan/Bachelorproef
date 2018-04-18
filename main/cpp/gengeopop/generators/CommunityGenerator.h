#pragma once

#include "../GeoGridConfig.h"
#include "PartialGenerator.h"

namespace gengeopop {

class CommunityGenerator : public PartialGenerator
{
public:
        explicit CommunityGenerator(stride::util::RNManager& rn_manager);

        void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
