#include "CollegeGenerator.h"
#include "gengeopop/College.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <gengeopop/College.h>
#include <iostream>
#include <queue>

namespace gengeopop {

void CollegeGenerator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        int  amountOfPupils  = geoGridConfig.calculated._1826_years_and_student;
        auto amountOfSchools = static_cast<int>(std::ceil(amountOfPupils / 3000.0)); // TODO magic constant

        std::vector<std::shared_ptr<Location>> cities = geoGrid->TopK(10);

        if (cities.empty()) {
                // trng can't handle empty vectors
                return;
        }

        int totalCitiesPopulation = 0;

        for (const std::shared_ptr<Location>& loc : cities) {
                totalCitiesPopulation += loc->GetPopulation();
        }

        std::vector<double> weights;

        for (const std::shared_ptr<Location>& loc : cities) {
                double weight =
                    static_cast<double>(loc->GetPopulation()) / static_cast<double>((double)totalCitiesPopulation);

                CheckWeight("CollegeGenerator", weight);

                weights.push_back(weight);
        }

        auto dist = m_rnManager.GetGenerator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int schoolId = 0; schoolId < amountOfSchools; schoolId++) {
                int                       locationId = dist();
                std::shared_ptr<Location> loc        = cities[locationId];
                auto highschool = std::make_shared<College>(geoGridConfig.generated.contactCenters++);
                highschool->Fill(geoGrid);
                loc->AddContactCenter(highschool);
        }
}

} // namespace gengeopop
