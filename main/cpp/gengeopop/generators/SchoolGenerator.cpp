#include "SchoolGenerator.h"
#include "../School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <iostream>

namespace gengeopop {

SchoolGenerator::SchoolGenerator(stride::util::RNManager& rn_manager) : PartialGenerator(rn_manager) {}

void SchoolGenerator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        /*
         * 1. calculate amount of schools, each school has average 500 pupils, taking in account the amount of pupils
         * 2. assign schools to a location by using a discrete distribution which reflects the relative amount of pupils
         * for that location the relative amount of pupils is equal to the relative amount of population
         */

        int  amountOfPupils  = geoGridConfig.calculated.compulsoryPupils;
        auto amountOfSchools = static_cast<int>(std::ceil(amountOfPupils / 500.0)); // TODO magic constant

        std::vector<double> weights;

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                weights.push_back(loc->getRelativePopulationSize());
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int schoolId = 0; schoolId < amountOfSchools; schoolId++) {
                int                       locationId = dist();
                std::shared_ptr<Location> loc        = (*geoGrid)[locationId];
                auto                      school = std::make_shared<School>(geoGridConfig.generated.contactCenters++);
                school->fill(geoGridConfig);
                loc->addContactCenter(school);
                //        std::cout << "Assign school " << schoolId << " to " << loc->getName() << std::endl;
        }
}

} // namespace gengeopop
