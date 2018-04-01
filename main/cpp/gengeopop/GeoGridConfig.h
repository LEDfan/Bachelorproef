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
        struct
        {
                /**
                 * Fraction of calc_fraction_1826 which are students.
                 */
                double fraction_1826_years_WhichAreStudents;

                /**
                 * Fraction of calc_1865_and_active which commute.
                 */
                double fraction_commutingPeople;

                /**
                 * Absolute size of the population.
                 */
                unsigned int populationSize;
        } input;

        /**
         * CALCULATED
         */
        struct
        {
                /**
                 * Absolute amount of population which are compulsory to go to school (i.e. 6-18 years old).
                 */
                unsigned int compulsoryPupils;

                /**
                 * Absolute amount of population which are [18, 65) years old.
                 */
                unsigned int _1865_years;

                /**
                 * Absolute amount of population which are [18, 26) years old.
                 */
                unsigned int _1826_years;

                /**
                 * Absolute amount of population which are [18, 26) years old and are a student.
                 */
                unsigned int _1826_years_and_student;

                /**
                 * Absolute amount of population which are [18, 65] years old and active.
                 */
                unsigned int _1865_and_years_active;

                /**
                 * Fraction of population which are [18, 65] years old and active.
                 */
                double fraction_1865_and_years_active;

                /**
                 * The number of households needed with this population size and these types of households.
                 */
                unsigned int households;
        } calculated;

        /*
         * CURRENT STATE
         */

        struct
        {
                /**
                 * A list of persons who go to Schools.
                 */
                std::vector<std::shared_ptr<stride::Person>> pupils;

                /**
                 * A list of persons who go to HighSchools.
                 */
                std::vector<std::shared_ptr<stride::Person>> students;

                /**
                 * A list of persons who are able to work at a Workplace.
                 */
                std::vector<std::shared_ptr<stride::Person>> workers;

                /**
                 * The current amount of ContactPools, used to obtain an Id for a new contactPool.
                 */
                unsigned int contactPools = 0;

                /**
                 * The current amount of ContactCenters, used to obtain an Id for a new contactCenter.
                 */
                unsigned int contactCenters = 0;

                /**
                 * A list of possible households.
                 */
                std::vector<std::shared_ptr<Household>> household_types;
        } generated;
};

} // namespace gengeopop
