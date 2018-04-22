#pragma once

#include "../GeoGridConfig.h"
#include "PartialPopulator.h"

namespace gengeopop {

class HighSchoolPopulator : public PartialPopulator
{
public:
        using PartialPopulator::PartialPopulator;

        void Apply(std::shared_ptr<GeoGrid> geogrid, GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
