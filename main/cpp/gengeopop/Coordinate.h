#pragma once

struct Coordinate
{
        Coordinate(double x, double y, double longitude, double latitude)
            : x(x), y(y), longitude(longitude), latitude(latitude)
        {
        }

        double x;
        double y;
        double longitude;
        double latitude;
};