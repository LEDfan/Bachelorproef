#pragma once

class GeoGridConfig {
public:

    unsigned int populationSize; // TODO move to GeoGrid and calculate it

    // fraction _ $people _ $condition
    double fraction_compulsoryPupils;
    double fraction_1826Years_WhichAreStudents;
    double fraction_Workables_WhichAreActive;

};
