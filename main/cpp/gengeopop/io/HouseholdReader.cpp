#include "HouseholdReader.h"
#include <vector>

namespace gengeopop {
HouseholdReader::HouseholdReader() : m_households() {}

const std::vector<std::shared_ptr<Household>>& gengeopop::HouseholdReader::getHouseHolds() const
{
        return m_households;
}
} // namespace gengeopop
