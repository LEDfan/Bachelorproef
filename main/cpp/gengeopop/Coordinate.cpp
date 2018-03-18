#include "Coordinate.h"

bool Coordinate::operator==(const Coordinate& other) const
{
        return x == other.x && y == other.y && longitude == other.longitude && latitude == other.latitude;
}
