#pragma  once

#include <string>
#include <gengeopop/Location.h>
#include <gengeopop/Household.h>
#include <vector>

namespace gengeopop {
    class HouseholdReader {
    public:
        std::vector<Household> getHouseHolds();

    protected:
        std::vector<Household> m_households;
    };
}

