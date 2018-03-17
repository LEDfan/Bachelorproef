#pragma once

#include "CitiesReader.h"

namespace gengeopop {
class CitiesCSVReader : public CitiesReader
{
public:
        CitiesCSVReader(std::istream& inputStream);

        void fillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;
};
} // namespace gengeopop
