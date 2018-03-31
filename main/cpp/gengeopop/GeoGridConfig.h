#pragma once
#include "io/HouseholdReader.h"
#include <cmath>
#include <gengeopop/io/CitiesReader.h>
#include <iomanip>
#include <iostream>
#include <util/StringUtils.h>

namespace gengeopop {

using stride::util::intToDottedString;

class GeoGridConfig
{
public:
        void Calculate(std::shared_ptr<GeoGrid> geoGrid, std::shared_ptr<HouseholdReader> householdReader);

        friend std::ostream& operator<<(std::ostream& stream, const GeoGridConfig& config);

        /**
         * INPUT
         */

        /**
         * Fraction of calc_fraction_1826 which are students.
         */
        double input_fraction_1826_years_WhichAreStudents;

        /**
         * Fraction of calc_1865_and_active which commute.
         */
        double input_fraction_commutingPeople;

        /**
         * Absolute size of the population.
         */
        unsigned int input_populationSize;

        // CALCULATED

        /**
         * Absolute amount of population which are compulsory to go to school (i.e. 6-18 years old).
         */
        unsigned int calc_compulsoryPupils;

        /**
         * Absolute amount of population which are [18, 65) years old.
         */
        unsigned int calc_1865_years;

        /**
         * Absolute amount of population which are [18, 26) years old.
         */
        unsigned int calc_1826_years;

        /**
         * Absolute amount of population which are [18, 26) years old and are a student.
         */
        unsigned int calc_1826_years_and_student;

        /**
         * Absolute amount of population which are [18, 65] years old and active.
         */
        unsigned int calc_1865_and_years_active;

        /**
         * Fraction of population which are [18, 65] years old and active.
         */
        double calc_fraction_1865_and_years_active;
};

} // namespace gengeopop
