#include <vector>
#include "HouseholdReader.h"

namespace gengeopop {
    const std::vector<std::shared_ptr<Household>>& gengeopop::HouseholdReader::getHouseHolds() const {
        return m_households;
    }
}
