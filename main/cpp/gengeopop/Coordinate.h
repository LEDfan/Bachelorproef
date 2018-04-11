#pragma once

struct Coordinate
{
        Coordinate(double x, double y, double longitude, double latitude)
            : x(x), y(y), longitude(longitude), latitude(latitude)
        {
        }

        Coordinate() : Coordinate(0, 0, 0, 0) {}

        Coordinate(double longitude, double latitude) : Coordinate(0, 0, longitude, latitude) {}

        double x;
        double y;
        double longitude;
        double latitude;

        bool operator==(const Coordinate& other) const;
};
