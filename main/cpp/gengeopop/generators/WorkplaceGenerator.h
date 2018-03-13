#pragma once

#include "PartialGenerator.h"
#include "../GeoGridConfig.h"

namespace gengeopop {

class WorkplaceGenerator : public PartialGenerator {
public:

    WorkplaceGenerator(stride::util::RNManager &rn_manager);

    void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;

};
}
