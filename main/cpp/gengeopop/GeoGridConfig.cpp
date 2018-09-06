/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

#include "GeoGridConfig.h"

namespace gengeopop {

using stride::util::intToDottedString;

GeoGridConfig::GeoGridConfig() : input{}, calculated{}, generated{}, constants{} {}

void GeoGridConfig::Calculate(std::shared_ptr<GeoGrid> geoGrid, std::shared_ptr<HouseholdReader> householdReader)
{
        calculated.compulsoryPupils = static_cast<unsigned int>(
            std::floor(householdReader->GetFractionCompulsoryPupils() * input.populationSize));
        calculated._1865_years =
            static_cast<unsigned int>(std::floor(householdReader->GetFraction1865Years() * input.populationSize));
        calculated._1826_years =
            static_cast<unsigned int>(std::floor(householdReader->GetFraction1826Years() * input.populationSize));
        calculated._1826_years_and_student =
            static_cast<unsigned int>(std::floor(input.fraction_1826_years_WhichAreStudents * calculated._1826_years));

        calculated._1865_and_years_active = static_cast<unsigned int>(std::floor(
            input.fraction_1865_years_active * (calculated._1865_years - calculated._1826_years_and_student)));

        calculated.households = static_cast<unsigned int>(
            std::floor(static_cast<double>(input.populationSize) / householdReader->AverageHouseholdSize()));

        generated.household_types = householdReader->GetHouseHolds();

        for (const std::shared_ptr<Location>& loc : *geoGrid) {
                loc->CalculatePopulation(input.populationSize);
        }
}

std::ostream& operator<<(std::ostream& out, const GeoGridConfig& config)
{
        const int width = 53;
        out << std::left << "Input:" << std::endl;
        out << std::left << std::setw(width) << "Fraction of active commuting"
            << config.input.fraction_active_commutingPeople << std::endl;
        out << std::left << std::setw(width) << "Fraction of students commuting"
            << config.input.fraction_student_commutingPeople << std::endl;
        out << std::left << std::setw(width) << "Fraction 18-65 (without students) which are active"
            << config.input.fraction_1865_years_active << std::endl;
        out << std::left << std::setw(width) << "Fraction 18-26 years which are students"
            << config.input.fraction_1826_years_WhichAreStudents << std::endl;
        out << std::left << std::setw(width) << "Population size" << intToDottedString(config.input.populationSize)
            << std::endl;
        out << std::endl;
        out << std::left << "Calculated:" << std::endl;
        out << std::left << std::setw(width) << "Compulsory pupils"
            << intToDottedString(config.calculated.compulsoryPupils) << std::endl;
        out << std::left << std::setw(width) << "18-26 years" << intToDottedString(config.calculated._1826_years)
            << std::endl;
        out << std::left << std::setw(width) << "18-26 years which are student"
            << intToDottedString(config.calculated._1826_years_and_student) << std::endl;
        out << std::left << std::setw(width) << "18-65 years" << intToDottedString(config.calculated._1865_years)
            << std::endl;
        out << std::left << std::setw(width) << "18-65 years which are active"
            << intToDottedString(config.calculated._1865_and_years_active) << std::endl;
        out << std::endl;
        return out;
}

} // namespace gengeopop
