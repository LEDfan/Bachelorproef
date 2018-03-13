#pragma once

#include "PartialGenerator.h"
#include "../GeoGridConfig.h"

namespace gengeopop {

class CommunityGenerator : public PartialGenerator {
public:

    CommunityGenerator(stride::util::RNManager &rn_manager);

    void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;

};
}
