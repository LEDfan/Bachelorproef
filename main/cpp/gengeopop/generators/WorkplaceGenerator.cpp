#include "WorkplaceGenerator.h"
#include "gengeopop/K12School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/Workplace.h>
#include <iostream>

namespace gengeopop {

void WorkplaceGenerator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        /*
         * 1. the amount of active employees and the amount of commuting people are given
         * 2. calculate the amount of workplaces, each workplace has an average of 20 employees
         * 3. calculate the amount of working people at each location = #residents + #incoming commuting people -
         * #outgoing commuting people
         * 4. use the last information for the distribution
         * 5. assign each workplace to a location
         */

        auto amountOfEmployees = geoGridConfig.calculated._1865_and_years_active;
        auto amountOfWorkplaces =
            static_cast<int>(std::ceil(amountOfEmployees / geoGridConfig.constants.meanWorkplaceSchoolSize));

        std::vector<double>
            weights; // = for each location #residents + #incoming commuting people - #outgoing commuting people

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                double amountOfWorkingPeople =
                    (loc->GetPopulation() +
                     loc->IncomingCommutingPeople(geoGridConfig.input.fraction_active_commutingPeople) -
                     loc->OutGoingCommutingPeople(geoGridConfig.input.fraction_active_commutingPeople) *
                         geoGridConfig.input.fraction_1865_years_active);

                double weight = amountOfWorkingPeople / amountOfEmployees;

                CheckWeight("WorkplaceGenerator", weight);

                weights.push_back(weight);
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        auto dist = m_rnManager[0].variate_generator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int workPlaceId = 0; workPlaceId < amountOfWorkplaces; workPlaceId++) {
                int                       locationId = dist();
                std::shared_ptr<Location> loc        = (*geoGrid)[locationId];
                auto workplace = std::make_shared<Workplace>(geoGridConfig.generated.contactCenters++);
                workplace->Fill(geoGrid);
                loc->AddContactCenter(workplace);
        }
}

} // namespace gengeopop
