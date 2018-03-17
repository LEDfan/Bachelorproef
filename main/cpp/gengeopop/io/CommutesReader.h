#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CommutesReader
{
public:
        CommutesReader(std::istream& inputStream);

        virtual void fillGeoGrid(std::shared_ptr<GeoGrid>) const = 0;

        virtual ~CommutesReader(){};

protected:
        std::istream& m_inputStream;
};
} // namespace gengeopop
