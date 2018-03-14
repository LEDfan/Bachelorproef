#pragma once

#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CitiesReader
{
public:
        CitiesReader();
        const std::map<int, std::shared_ptr<Location>>& getLocations() const;

protected:
        std::map<int, std::shared_ptr<Location>> m_locations;
};
} // namespace gengeopop
