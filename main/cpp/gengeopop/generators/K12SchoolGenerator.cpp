/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

#include "K12SchoolGenerator.h"
#include "gengeopop/K12School.h"
#include <trng/discrete_dist.hpp>
#include <trng/lcg64.hpp>
#include <cmath>
#include <gengeopop/GeoGridConfig.h>
#include <iostream>

namespace gengeopop {

void K12SchoolGenerator::Apply(std::shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        /*
         * 1. calculate amount of schools, each school has average 500 pupils, taking in account the amount of pupils
         * 2. assign schools to a location by using a discrete distribution which reflects the relative amount of pupils
         * for that location the relative amount of pupils is equal to the relative amount of population
         */

        int  amountOfPupils  = geoGridConfig.calculated.compulsoryPupils;
        auto amountOfSchools = static_cast<int>(std::ceil(amountOfPupils / geoGridConfig.constants.meanK12SchoolSize));

        std::vector<double> weights;

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                weights.push_back(loc->GetRelativePopulationSize());
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        auto dist = m_rnManager[0].variate_generator(trng::discrete_dist(weights.begin(), weights.end()));

        for (int schoolId = 0; schoolId < amountOfSchools; schoolId++) {
                int                       locationId = dist();
                std::shared_ptr<Location> loc        = (*geoGrid)[locationId];
                auto k12School = std::make_shared<K12School>(geoGridConfig.generated.contactCenters++);
                k12School->Fill(geoGrid);
                loc->AddContactCenter(k12School);
        }
}

} // namespace gengeopop
