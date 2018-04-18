#pragma once

#include "../GeoGridConfig.h"
#include "PartialGenerator.h"

namespace gengeopop {

class HighSchoolGenerator : public PartialGenerator
{
public:
        explicit HighSchoolGenerator(stride::util::RNManager& rn_manager);

        void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig &geoGridConfig) override;
};
} // namespace gengeopop
