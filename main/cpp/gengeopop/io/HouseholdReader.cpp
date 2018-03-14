#include "HouseholdReader.h"
#include <vector>

namespace gengeopop {
const std::vector<std::shared_ptr<Household>>& gengeopop::HouseholdReader::getHouseHolds() const
{
        return m_households;
}
} // namespace gengeopop
