#pragma once

#include "CitiesReader.h"
#include "CommutesReader.h"
#include "HouseholdReader.h"
#include <string>

namespace gengeopop {
class ReaderFactory
{
public:
        CitiesReader    createCitiesReader(std::string filename);
        CommutesReader  createCommutesReader(std::string filename);
        HouseholdReader createHouseholdReader(std::string filename);
};
} // namespace gengeopop
