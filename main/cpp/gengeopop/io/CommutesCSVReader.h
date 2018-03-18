#pragma once

#include "CommutesReader.h"
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CommutesCSVReader : public CommutesReader
{
public:
        CommutesCSVReader(std::unique_ptr<std::istream> istream);
        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;
};
} // namespace gengeopop
