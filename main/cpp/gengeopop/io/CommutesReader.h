#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CommutesReader
{
public:
        explicit CommutesReader(std::unique_ptr<std::istream> inputStream);

        virtual void FillGeoGrid(std::shared_ptr<GeoGrid>) const = 0;

        virtual ~CommutesReader() = default;

protected:
        std::unique_ptr<std::istream> m_inputStream;
};
} // namespace gengeopop
