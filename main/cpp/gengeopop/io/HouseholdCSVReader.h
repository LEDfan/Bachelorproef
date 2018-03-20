#pragma once

#include "HouseholdReader.h"

namespace gengeopop {
class HouseholdCSVReader : public HouseholdReader
{
public:
        HouseholdCSVReader(std::unique_ptr<std::istream> inputStream);
};
} // namespace gengeopop
