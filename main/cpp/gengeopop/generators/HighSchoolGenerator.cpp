#include "HighSchoolGenerator.h"
#include "../HighSchool.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <gengeopop/HighSchool.h>
#include <iostream>
#include <queue>

namespace gengeopop {

HighSchoolGenerator::HighSchoolGenerator(stride::util::RNManager& rn_manager) : PartialGenerator(rn_manager) {}

void HighSchoolGenerator::apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        int amountOfPupils =
            std::floor(geoGridConfig.populationSize * geoGridConfig.fraction_1826Years_WhichAreStudents);
        int amountOfSchools = std::ceil(amountOfPupils / 3000.0); // TODO magic constant

        std::vector<std::shared_ptr<Location>> cities = geoGrid->topK(10);

        if (cities.empty()) {
                // trng can't handle empty vectors
                return;
        }

        int totalCitiesPopulation = 0;

        for (const std::shared_ptr<Location>& loc : cities) {
                totalCitiesPopulation += loc->getPopulation();
        }

        std::vector<double> weights;

        for (const std::shared_ptr<Location>& loc : cities) {
                weights.push_back((double)loc->getPopulation() / (double)totalCitiesPopulation);
        }

        auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int schoolId = 0; schoolId < amountOfSchools; schoolId++) {
                int                       locationId = dist();
                std::shared_ptr<Location> loc        = cities[locationId];
                loc->addContactCenter(std::make_shared<HighSchool>());
        }
}

} // namespace gengeopop
