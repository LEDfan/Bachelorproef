#include "Location.h"
#include <cmath>
#include <utility>

namespace gengeopop {
Location::Location(unsigned int id, unsigned int province, unsigned int population, Coordinate coordinate,
                   std::string name)
    : m_id(id), m_name(std::move(name)), m_province(province), m_population(population), m_coordinate(coordinate),
      m_contactCenters(), m_incomingCommutingLocations(), m_outgoingCommutingLocations()
{
}

std::string Location::getName() const { return m_name; }

unsigned int Location::getProvince() const { return m_province; }

unsigned int Location::getID() const { return m_id; }

unsigned int Location::getPopulation() const { return m_population; }

void Location::addContactCenter(std::shared_ptr<ContactCenter> contactCenter)
{
        m_contactCenters.push_back(contactCenter);
}

const std::vector<std::shared_ptr<ContactCenter>>& Location::getContactCenters() const { return m_contactCenters; }

const Coordinate& Location::getCoordinate() const { return m_coordinate; }

Location::iterator Location::begin() { return m_contactCenters.begin(); }

Location::iterator Location::end() { return m_contactCenters.end(); }

const std::vector<std::pair<std::shared_ptr<Location>, double>>& Location::getIncomingCommuningCities() const
{
        return m_incomingCommutingLocations;
}

void Location::addIncomingCommutingLocation(std::shared_ptr<Location> location, double proportion)
{
        m_incomingCommutingLocations.emplace_back(location, proportion);
}

const std::vector<std::pair<std::shared_ptr<Location>, double>>& Location::getOutgoingCommuningCities() const
{
        return m_outgoingCommutingLocations;
}

void Location::addOutgoingCommutingLocation(std::shared_ptr<Location> location, double proportion)
{
        m_outgoingCommutingLocations.emplace_back(location, proportion);
}

int Location::incomingCommutingPeople(double fractionOfPopulationCommuting) const
{
        double value = 0;
        for (const auto& locProportion : m_incomingCommutingLocations) {
                // locProportion.second of the people in locProportion.first are commuting to this
                value += locProportion.second *
                         (fractionOfPopulationCommuting * (double)locProportion.first->getPopulation());
        }
        return static_cast<int>(std::floor(value));
}

int Location::outGoingCommutingPeople(double fractionOfPopulationCommuting) const
{
        double totalProportion = 0;
        for (const auto& locProportion : m_outgoingCommutingLocations) {
                // locProportion.second of the people in this are commuting to locProportion.first
                totalProportion += locProportion.second;
        }
        return static_cast<int>(
            std::floor(totalProportion * (fractionOfPopulationCommuting * static_cast<double>(m_population))));
}

bool Location::operator==(const Location& other) const
{
        return getID() == other.getID() && getCoordinate() == other.getCoordinate() && getName() == other.getName() &&
               getProvince() == other.getProvince() && getPopulation() == other.getPopulation() &&
               getContactCenters() == other.getContactCenters() &&
               getIncomingCommuningCities() == other.getIncomingCommuningCities() &&
               getOutgoingCommuningCities() == other.getOutgoingCommuningCities();
}

} // namespace gengeopop
