#pragma once
#include "io/HouseholdReader.h"
#include <cmath>
#include <gengeopop/io/CitiesReader.h>

namespace gengeopop {

class GeoGridConfig
{
public:
        void Calculate(std::shared_ptr<HouseholdReader> householdReader, std::shared_ptr<CitiesReader> citiesReader)
        {
                calc_populationSize   = citiesReader->getTotalPopulation();
                calc_compulsoryPupils = static_cast<unsigned int>(
                    std::floor(householdReader->getFractionCompulsoryPupils() * calc_populationSize));
                calc_1865_years = static_cast<unsigned int>(
                    std::floor(householdReader->getFraction1865Years() * calc_populationSize));
                calc_1826_years = static_cast<unsigned int>(
                    std::floor(householdReader->getFraction1826Years() * calc_populationSize));
                calc_1826_years_and_student =
                    static_cast<unsigned int>(std::floor(input_fraction_1826_years_WhichAreStudents * calc_1826_years));
                calc_1865_and_years_active = calc_1865_years - calc_1826_years_and_student;
                calc_fraction_1865_and_years_active =
                    static_cast<double>(calc_1865_and_years_active) / static_cast<double>(calc_populationSize);
        }

        void ToSteam(std::ostream& out) const { out << "test" << std::endl; }

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

        // CALCULATED

        /**
         * The total amount of people currently in this GeoGrid.
         * Calculated by using the CitiesReader.
         */
        unsigned int calc_populationSize;

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
