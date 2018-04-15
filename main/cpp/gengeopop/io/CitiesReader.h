#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CitiesReader
{
public:
        explicit CitiesReader(std::unique_ptr<std::istream> inputStream);

        virtual void FillGeoGrid(std::shared_ptr<GeoGrid>) const = 0;

        virtual ~CitiesReader() = default;

protected:
        std::unique_ptr<std::istream> m_inputStream;
};
} // namespace gengeopop
