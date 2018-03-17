#pragma once

#include <gengeopop/Location.h>
#include <map>
#include <string>
#include "CommutesReader.h"

namespace gengeopop {
class CommutesCSVReader : public CommutesReader
{
public:
        CommutesCSVReader(std::istream& inputStream);
        void fillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;
};
} // namespace gengeopop
