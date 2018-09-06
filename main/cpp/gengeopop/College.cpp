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

#include "College.h"
#include "GeoGridConfig.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

College::College(unsigned int id) : ContactCenter(id) {}

std::string College::GetType() const { return "College"; }

unsigned int College::GetMaxPools() const { return 20; }

unsigned int College::GetPoolSize() const { return 150; }

void College::Fill(const std::shared_ptr<GeoGrid>& geoGrid)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(geoGrid->CreateContactPool(Id::College));
        }
}
} // namespace gengeopop
