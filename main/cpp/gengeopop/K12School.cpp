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

#include "K12School.h"
#include "GeoGridConfig.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

K12School::K12School(unsigned int id) : ContactCenter(id) {}

unsigned int K12School::GetMaxPools() const { return 25; }

unsigned int K12School::GetPoolSize() const { return 20; }

std::string K12School::GetType() const { return "K12School"; }

void K12School::Fill(const std::shared_ptr<GeoGrid>& geoGrid)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(geoGrid->CreateContactPool(Id::K12School));
        }
}

} // namespace gengeopop
