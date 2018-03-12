#pragma once

#include "PartialGenerator.h"
#include "../GeoGridConfig.h"

namespace gengeopop {

class HighSchoolGenerator : public PartialGenerator {
public:

    HighSchoolGenerator(stride::util::RNManager &rn_manager);

    void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;

};
}
