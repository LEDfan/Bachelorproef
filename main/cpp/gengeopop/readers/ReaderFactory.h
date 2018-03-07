#pragma  once

#include <string>
#include "CommutesReader.h"
#include "CitiesReader.h"
#include "HouseholdReader.h"

namespace gengeopop {
    class ReaderFactory {
    public:
        CitiesReader createCitiesReader(std::string filename);
        CommutesReader createCommutesReader(std::string filename);
        HouseholdReader createHouseholdReader(std::string filename);
    };
}


