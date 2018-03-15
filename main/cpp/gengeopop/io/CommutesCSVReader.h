#pragma once

#include <gengeopop/Location.h>
#include <map>
#include <string>
#include "CommutesReader.h"

namespace gengeopop {
class CommutesCSVReader : public CommutesReader
{
public:
        CommutesCSVReader(std::istream& inputStream, const std::map<int, std::shared_ptr<Location>>& locations);
};
} // namespace gengeopop
