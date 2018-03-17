#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CitiesReader
{
public:
        CitiesReader(std::istream& inputStream);

        virtual void fillGeoGrid(std::shared_ptr<GeoGrid>) const = 0;

protected:
        std::istream& m_inputStream;
};
} // namespace gengeopop
