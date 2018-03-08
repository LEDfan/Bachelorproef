#pragma once

#include "HouseholdReader.h"

namespace gengeopop {
   class HouseholdCSVReader : public HouseholdReader {
   public:
       HouseholdCSVReader(std::istream& inputStream);

    };
}

