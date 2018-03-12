#include "Household.h"

unsigned int gengeopop::Household::getMaxPools() {
    return 1;
}

unsigned int gengeopop::Household::getPoolSize() {
    return 15;
}

std::string gengeopop::Household::getType() {
    return "Household";
}
