#include <vector>
#include "HouseholdReader.h"

namespace gengeopop {
    std::vector <std::shared_ptr<Household>> gengeopop::HouseholdReader::getHouseHolds() {
        return m_households;
    }
}
