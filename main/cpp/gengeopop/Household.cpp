#include "Household.h"

namespace gengeopop {

unsigned int Household::getMaxPools() const { return 1; }

unsigned int Household::getPoolSize() const { return 15; }

std::string Household::getType() const { return "Household"; }
} // namespace gengeopop
