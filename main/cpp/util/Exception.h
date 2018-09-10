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

#ifndef STRIDE_EXCEPTION_H
#define STRIDE_EXCEPTION_H

#include <stdexcept>

namespace stride {
namespace util {

/// Basic exception class. Needed to prevent clang-tidy warning: "thrown exception type is not nothrow copy
/// constructible"
class Exception : public std::runtime_error
{
public:
        explicit Exception(const std::string& msg);
};
} // namespace util

} // namespace stride

#endif // STRIDE_EXCEPTION_H
