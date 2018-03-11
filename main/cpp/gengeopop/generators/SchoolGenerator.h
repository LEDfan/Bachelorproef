#pragma once

#include "IPartialGenerator.h"
#include "../GeoGridConfig.h"

namespace gengeopop {

class SchoolGenerator : public IPartialGenerator {
public:

    void apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;

};
}
