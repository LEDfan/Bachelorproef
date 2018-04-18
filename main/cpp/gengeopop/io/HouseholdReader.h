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
        HouseholdReader();
        const std::vector<std::shared_ptr<Household>>& GetHouseHolds() const;

        double       GetFractionCompulsoryPupils() const;
        double       GetFraction1826Years() const;
        double       GetFraction1865Years() const;
        unsigned int GetTotalPersonsInHouseholds() const;
        double       AverageHouseholdSize() const;
        virtual ~HouseholdReader() = default;

protected:
        std::vector<std::shared_ptr<Household>> m_households;
        unsigned int                            m_totalCompulsory = 0;
        unsigned int                            m_total1826Years  = 0;
        unsigned int                            m_total1865Years  = 0;
        unsigned int                            m_total           = 0;
};
} // namespace gengeopop
