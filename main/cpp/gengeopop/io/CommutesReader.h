#pragma once

#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CommutesReader
{
public:
        std::map<Location, std::map<Location, unsigned int>> getCommutes();

private:
        std::map<Location, std::map<Location, unsigned int>> m_commutes;
};
} // namespace gengeopop
