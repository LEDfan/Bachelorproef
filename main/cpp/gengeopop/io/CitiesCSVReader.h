#pragma once

#include "CitiesReader.h"

namespace gengeopop {
class CitiesCSVReader : public CitiesReader
{
public:
        explicit CitiesCSVReader(std::unique_ptr<std::istream> inputStream);

        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;
};
} // namespace gengeopop
