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
 *  Copyright 2018, Niels Aerens, Thomas Avé, Jan Broeckhove, Tobia De Koninck, Robin Jadoul
 */

#include <gengeopop/io/GeoGridReader.h>

namespace gengeopop {

GeoGridReader::GeoGridReader(std::unique_ptr<std::istream> inputStream, stride::Population* pop, std::size_t regionId,
                             std::string regionName)
    : m_people(), m_commutes(), m_regionId(regionId), m_regionName(regionName), m_inputStream(std::move(inputStream)),
      m_population(pop)
{
}
void GeoGridReader::AddCommutes(std::shared_ptr<GeoGrid> geoGrid)
{
        for (const auto& commute_tuple : m_commutes) {
                auto a      = geoGrid->GetById(std::get<0>(commute_tuple));
                auto b      = geoGrid->GetById(std::get<1>(commute_tuple));
                auto amount = std::get<2>(commute_tuple);
                a->AddOutgoingCommutingLocation(b, amount);
                b->AddIncomingCommutingLocation(a, amount);
        }
}

} // namespace gengeopop
