#pragma once

#include "CommutesReader.h"
#include <gengeopop/Location.h>
#include <map>
#include <string>
#include <util/CSV.h>

namespace gengeopop {
class CommutesCSVReader : public CommutesReader
{
public:
        CommutesCSVReader(std::unique_ptr<std::istream> istream);
        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;

private:
        stride::util::CSV m_reader;
};
} // namespace gengeopop
