#pragma  once

#include <string>
#include <gengeopop/Location.h>
#include <gengeopop/Household.h>
#include <vector>
#include <memory>

namespace gengeopop {
    class HouseholdReader {
    public:
        const std::vector<std::shared_ptr<Household>>& getHouseHolds() const;

    protected:
        std::vector<std::shared_ptr<Household>> m_households;
    };
}

