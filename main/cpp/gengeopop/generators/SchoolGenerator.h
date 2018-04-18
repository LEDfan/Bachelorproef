#pragma once

#include "../GeoGridConfig.h"
#include "PartialGenerator.h"

namespace gengeopop {

class SchoolGenerator : public PartialGenerator
{
public:
        explicit SchoolGenerator(stride::util::RNManager& rn_manager);

        void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
