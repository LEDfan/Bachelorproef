#include "Coordinate.h"
#include <tuple>

bool Coordinate::operator==(const Coordinate& other) const
{
        return std::make_tuple(x, y, longitude, latitude) ==
               std::make_tuple(other.x, other.y, other.longitude, other.latitude);
}
