#pragma once

class GeoGridConfig
{
public:
        unsigned int populationSize; // TODO move to GeoGrid and calculate it

        // fraction _ $people _ $condition
        double fraction_compulsoryPupils;
        double fraction_1826Years_WhichAreStudents; // relative to amount of 1826 or students?
        double fraction_Workables_WhichAreActive;   // relative to amount of working people or populatoin?
        double fraction_CommutingPeople;            // relative to amount of active working people or population?
};
