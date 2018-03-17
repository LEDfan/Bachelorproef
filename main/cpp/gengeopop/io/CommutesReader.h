#pragma once

#include <gengeopop/Location.h>
#include <map>
#include <string>
#include <gengeopop/GeoGrid.h>

namespace gengeopop {
class CommutesReader
{
public:
        CommutesReader(std::istream& inputStream);

        virtual void fillGeoGrid(std::shared_ptr<GeoGrid> ) const = 0;

protected:
        std::istream&                                                                     m_inputStream;
};
} // namespace gengeopop
