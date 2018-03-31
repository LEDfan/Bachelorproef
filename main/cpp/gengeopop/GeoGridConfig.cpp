#include "GeoGridConfig.h"

namespace gengeopop {

using stride::util::intToDottedString;

void GeoGridConfig::Calculate(std::shared_ptr<GeoGrid> geoGrid, std::shared_ptr<HouseholdReader> householdReader)
{
        calc_compulsoryPupils = static_cast<unsigned int>(
            std::floor(householdReader->getFractionCompulsoryPupils() * input_populationSize));
        calc_1865_years =
            static_cast<unsigned int>(std::floor(householdReader->getFraction1865Years() * input_populationSize));
        calc_1826_years =
            static_cast<unsigned int>(std::floor(householdReader->getFraction1826Years() * input_populationSize));
        calc_1826_years_and_student =
            static_cast<unsigned int>(std::floor(input_fraction_1826_years_WhichAreStudents * calc_1826_years));
        calc_1865_and_years_active = calc_1865_years - calc_1826_years_and_student;
        calc_fraction_1865_and_years_active =
            static_cast<double>(calc_1865_and_years_active) / static_cast<double>(input_populationSize);
        calc_households = static_cast<unsigned int>(
            std::floor(static_cast<double>(input_populationSize) / householdReader->averageHouseholdSize()));

        household_types = householdReader->GetHouseHolds();

        for (std::shared_ptr<Location> loc : *geoGrid) {
                loc->calculatePopulation(input_populationSize);
        }
}

std::ostream& operator<<(std::ostream& out, const GeoGridConfig& config)
{
        out << std::left << "Input:" << std::endl;
        out << std::left << std::setw(40) << "Fraction commuting" << config.input_fraction_commutingPeople << std::endl;
        out << std::left << std::setw(40) << "Fraction 18-26 years which are students"
            << config.input_fraction_1826_years_WhichAreStudents << std::endl;
        out << std::left << std::setw(40) << "Population size" << intToDottedString(config.input_populationSize)
            << std::endl;
        out << std::endl;
        out << std::left << "Calculated:" << std::endl;
        out << std::left << std::setw(40) << "Compulsory pupils" << intToDottedString(config.calc_compulsoryPupils)
            << std::endl;
        out << std::left << std::setw(40) << "18-26 years" << intToDottedString(config.calc_1826_years) << std::endl;
        out << std::left << std::setw(40) << "18-26 years which are student"
            << intToDottedString(config.calc_1826_years_and_student) << std::endl;
        out << std::left << std::setw(40) << "18-65 years" << intToDottedString(config.calc_1865_years) << std::endl;
        out << std::left << std::setw(40) << "18-65 years which are active"
            << intToDottedString(config.calc_1865_and_years_active) << std::endl;
        out << std::endl;
        return out;
}

} // namespace gengeopop
