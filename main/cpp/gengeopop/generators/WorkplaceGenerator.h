#pragma once

#include "../GeoGridConfig.h"
#include "PartialGenerator.h"

namespace gengeopop {

/// Generate Workplaces
class WorkplaceGenerator : public PartialGenerator
{
public:
        using PartialGenerator::PartialGenerator;

        void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
