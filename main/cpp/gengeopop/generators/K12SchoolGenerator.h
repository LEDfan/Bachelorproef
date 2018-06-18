#pragma once

#include "../GeoGridConfig.h"
#include "PartialGenerator.h"

namespace gengeopop {

/// Generate K12Schools
class K12SchoolGenerator : public PartialGenerator
{
public:
        using PartialGenerator::PartialGenerator;

        void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
