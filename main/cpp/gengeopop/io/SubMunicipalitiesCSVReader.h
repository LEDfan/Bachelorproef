#pragma once

#include "SubMunicipalitiesReader.h"

namespace gengeopop {
class SubMunicipalitiesCSVReader : public SubMunicipalitiesReader
{
public:
        SubMunicipalitiesCSVReader(std::unique_ptr<std::istream> inputStream);

        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;
};
} // namespace gengeopop
