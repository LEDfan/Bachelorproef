#pragma once

#include "HouseholdReader.h"
#include <pop/Population.h>

namespace gengeopop {
class HouseholdCSVReader : public HouseholdReader
{
public:
        HouseholdCSVReader(std::unique_ptr<std::istream> inputStream);

private:
        std::vector<stride::Person*> m_persons;
};
} // namespace gengeopop
