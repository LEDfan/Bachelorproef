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

#pragma once
#include <gengeopop/io/GeoGridReader.h>
#include <string>

namespace gengeopop {

/**
 * A Factory for creating the correct implementation of a GeoGridReader based on the filename extension
 */
class GeoGridReaderFactory
{
public:
        /// Create a GeoGridReader based on the provided extension in the filename
        std::shared_ptr<GeoGridReader> CreateReader(std::string filename) const;
};
} // namespace gengeopop
