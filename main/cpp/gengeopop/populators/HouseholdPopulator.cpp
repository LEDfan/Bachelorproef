#include "HouseholdPopulator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <gengeopop/Household.h>
#include <iostream>

namespace gengeopop {

HouseholdPopulator::HouseholdPopulator(stride::util::RNManager& rn_manager) : PartialPopulator(rn_manager) {}

void HouseholdPopulator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
//        auto dist = m_rnManager.GetPopulator(trng::discrete_dist(weights.begin(), weights.end()));
}

} // namespace gengeopop
