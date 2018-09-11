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

#include "GeoGridWriterFactory.h"
#include "GeoGridJSONWriter.h"
#include "GeoGridProtoWriter.h"

#if __has_include(<filesystem>)
#include <filesystem>
#else
#include <experimental/filesystem>
namespace std {
namespace filesystem = std::experimental::filesystem;
}
#endif

#include <iostream>
#include <util/Exception.h>
namespace gengeopop {

std::shared_ptr<GeoGridWriter> GeoGridWriterFactory::CreateWriter(std::string filename) const
{
        std::filesystem::path path(filename);

        if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONWriter>();
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<GeoGridProtoWriter>();
        } else {
                throw stride::util::Exception("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
