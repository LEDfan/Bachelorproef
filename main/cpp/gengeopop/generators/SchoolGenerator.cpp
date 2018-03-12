#include "SchoolGenerator.h"
#include "../School.h"
#include <iostream>
#include <cmath>
#include <trng/lcg64.hpp>
#include <trng/discrete_dist.hpp>


namespace gengeopop {

SchoolGenerator::SchoolGenerator(stride::util::RNManager &rn_manager) : PartialGenerator(rn_manager) {

}

void SchoolGenerator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig) {
    /*
     * 1. calculate amount of schools, each school has average 500 pupils, taking in account the amount of pupils
     * 2. assign schools to a location by using a discrete distribution which reflects the relative amount of pupils for that location
     * the relative amount of pupils is equal to the relative amount of population
     */

    int amountOfPupils = std::floor(geoGridConfig.populationSize * geoGridConfig.fraction_compulsoryPupils);
    int amountOfSchools = std::ceil(amountOfPupils / 500.0); // TODO magic constant

    std::vector<double> weights;

    for (std::shared_ptr<Location> loc : *geoGrid) {
        weights.push_back((double) loc->getPopulation() / (double) geoGridConfig.populationSize);
    }

        if (weights.size() == 0) {
                // trng can't handle empty vectors
                return;
        }


    auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));

    for (int schoolId = 0; schoolId < amountOfSchools; schoolId++) {
        int locationId = dist();
        std::shared_ptr<Location> loc = (*geoGrid)[locationId];
        loc->addContactCenter(std::make_shared<School>());
//        std::cout << "Assign school " << schoolId << " to " << loc->getName() << std::endl;
    }

}

}
