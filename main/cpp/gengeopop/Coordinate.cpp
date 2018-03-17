#include "Coordinate.h"

bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
        return  lhs.x == rhs.x
                && lhs.y == rhs.y
                && lhs.longitude == rhs.longitude
                && lhs.latitude == rhs.latitude;
}
