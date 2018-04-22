#include "HouseholdReader.h"
#include <vector>

namespace gengeopop {
HouseholdReader::HouseholdReader() : m_households() {}

const std::vector<std::shared_ptr<Household>>& gengeopop::HouseholdReader::GetHouseHolds() const
{
        return m_households;
}

unsigned int HouseholdReader::GetTotalPersonsInHouseholds() const { return m_total; }

double HouseholdReader::AverageHouseholdSize() const
{
        return static_cast<double>(m_total) / static_cast<double>(m_households.size());
}

double HouseholdReader::GetFraction1826Years() const
{
        return static_cast<double>(m_total1826Years) / static_cast<double>(m_total);
}

double HouseholdReader::GetFractionCompulsoryPupils() const
{
        return static_cast<double>(m_totalCompulsory) / static_cast<double>(m_total);
}

double HouseholdReader::GetFraction1865Years() const
{
        return static_cast<double>(m_total1865Years) / static_cast<double>(m_total);
}

} // namespace gengeopop
