#pragma once

#include "CitiesReader.h"

namespace gengeopop {
class CitiesCSVReader : public CitiesReader
{
public:
        CitiesCSVReader(std::unique_ptr<std::istream> inputStream);

        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) override;
};
} // namespace gengeopop
