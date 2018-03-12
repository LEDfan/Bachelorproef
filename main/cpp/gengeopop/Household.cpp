#include "Household.h"

unsigned int gengeopop::Household::getMaxPools() const {
    return 1;
}

unsigned int gengeopop::Household::getPoolSize() const {
    return 15;
}

std::string gengeopop::Household::getType() {
    return "Household";
}
