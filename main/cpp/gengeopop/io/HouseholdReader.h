#pragma once

#include <gengeopop/Household.h>
#include <gengeopop/Location.h>
#include <memory>
#include <string>
#include <vector>

namespace gengeopop {
class HouseholdReader
{
public:
        const std::vector<std::shared_ptr<Household>>& getHouseHolds() const;

protected:
        std::vector<std::shared_ptr<Household>> m_households;
};
} // namespace gengeopop
