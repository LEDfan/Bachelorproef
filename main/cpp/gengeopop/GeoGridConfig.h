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
        void Calculate(std::shared_ptr<GeoGrid> geoGrid, std::shared_ptr<HouseholdReader> householdReader)
        {
                calc_compulsoryPupils = static_cast<unsigned int>(
                    std::floor(householdReader->getFractionCompulsoryPupils() * input_populationSize));
                calc_1865_years = static_cast<unsigned int>(
                    std::floor(householdReader->getFraction1865Years() * input_populationSize));
                calc_1826_years = static_cast<unsigned int>(
                    std::floor(householdReader->getFraction1826Years() * input_populationSize));
                calc_1826_years_and_student =
                    static_cast<unsigned int>(std::floor(input_fraction_1826_years_WhichAreStudents * calc_1826_years));
                calc_1865_and_years_active = calc_1865_years - calc_1826_years_and_student;
                calc_fraction_1865_and_years_active =
                    static_cast<double>(calc_1865_and_years_active) / static_cast<double>(input_populationSize);

                for (const std::shared_ptr<Location> &loc : *geoGrid) {
                        loc->calculatePopulation(input_populationSize);
                }
        }

        void ToStream(std::ostream& out) const
        {
                out << std::left << "Input:" << std::endl;
                out << std::left << std::setw(40) << "Fraction commuting" << input_fraction_commutingPeople
                    << std::endl;
                out << std::left << std::setw(40) << "Fraction 18-26 years which are students"
                    << input_fraction_1826_years_WhichAreStudents << std::endl;
                out << std::left << std::setw(40) << "Population size" << intToDottedString(input_populationSize)
                    << std::endl;
                out << std::endl;
                out << std::left << "Calculated:" << std::endl;
                out << std::left << std::setw(40) << "Compulsory pupils" << intToDottedString(calc_compulsoryPupils)
                    << std::endl;
                out << std::left << std::setw(40) << "18-26 years" << intToDottedString(calc_1826_years) << std::endl;
                out << std::left << std::setw(40) << "18-26 years which are student"
                    << intToDottedString(calc_1826_years_and_student) << std::endl;
                out << std::left << std::setw(40) << "18-65 years" << intToDottedString(calc_1865_years) << std::endl;
                out << std::left << std::setw(40) << "18-65 years which are active"
                    << intToDottedString(calc_1865_and_years_active) << std::endl;
                out << std::endl;
        }

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
