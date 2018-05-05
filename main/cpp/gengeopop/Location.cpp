#include "Location.h"
#include <Exception.h>
#include <cmath>

namespace gengeopop {
Location::Location(unsigned int id, unsigned int province, Coordinate coordinate, std::string name)
    : m_id(id), m_name(std::move(name)), m_province(province), m_population(0), m_relativePopulation(0.0),
      m_coordinate(coordinate), m_contactCenters(), m_incomingCommutingLocations(), m_outgoingCommutingLocations(),
      m_subMunicipalities(), m_parent(nullptr), m_contactCenterByType()
{
}

Location::Location(unsigned int id, unsigned int province, unsigned int population, Coordinate coordinate,
                   std::string name)
    : Location(id, province, coordinate, std::move(name))
{
        m_population = population;
}

std::string Location::GetName() const { return m_name; }

unsigned int Location::GetProvince() const { return m_province; }

unsigned int Location::GetID() const { return m_id; }

unsigned int Location::GetPopulation() const { return m_population; }

double Location::GetInfectedRatio() const
{
        unsigned int infected   = 0;
        unsigned int population = 0;

        for (const std::shared_ptr<gengeopop::ContactCenter>& cc : m_contactCenters) {
                auto r = cc->GetPopulationAndInfectedCount();
                population += r.first;
                infected += r.second;
        }

        if (GetPopulation() == 0) {
                return 0;
        }

        double r = static_cast<double>(infected) / static_cast<double>(population);

        return r;
}

double Location::GetInfectedRatioOfSubmunicipalities() const
{
        unsigned int infected   = 0;
        unsigned int population = 0;
        for (auto loc : m_subMunicipalities) {
                for (const std::shared_ptr<gengeopop::ContactCenter>& cc : loc->GetContactCenters()) {
                        auto r = cc->GetPopulationAndInfectedCount();
                        population += r.first;
                        infected += r.second;
                }
        }

        if (population == 0) {
                return 0;
        }

        return static_cast<double>(infected) / static_cast<double>(population);
}

const std::vector<std::shared_ptr<ContactCenter>>& Location::GetContactCenters() const { return m_contactCenters; }

const Coordinate& Location::GetCoordinate() const { return m_coordinate; }

Location::iterator Location::begin() { return m_contactCenters.begin(); }

Location::iterator Location::end() { return m_contactCenters.end(); }

const std::vector<std::pair<std::shared_ptr<Location>, double>>& Location::GetIncomingCommuningCities() const
{
        return m_incomingCommutingLocations;
}

void Location::AddIncomingCommutingLocation(std::shared_ptr<Location> location, double proportion)
{
        m_incomingCommutingLocations.emplace_back(location, proportion);
}

const std::vector<std::pair<std::shared_ptr<Location>, double>>& Location::GetOutgoingCommuningCities() const
{
        return m_outgoingCommutingLocations;
}

void Location::AddOutgoingCommutingLocation(std::shared_ptr<Location> location, double proportion)
{
        m_outgoingCommutingLocations.emplace_back(location, proportion);
}

int Location::IncomingCommutingPeople(double fractionOfPopulationCommuting) const
{
        double value = 0;
        for (const auto& locProportion : m_incomingCommutingLocations) {
                // locProportion.second of the people in locProportion.first are commuting to this
                value += locProportion.second *
                         (fractionOfPopulationCommuting * (double)locProportion.first->GetPopulation());
        }
        return static_cast<int>(std::floor(value));
}

int Location::OutGoingCommutingPeople(double fractionOfPopulationCommuting) const
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
        auto        sub1 = GetSubMunicipalities();
        const auto& sub2 = other.GetSubMunicipalities();

        return GetID() == other.GetID() && GetCoordinate() == other.GetCoordinate() && GetName() == other.GetName() &&
               GetProvince() == other.GetProvince() && GetPopulation() == other.GetPopulation() &&
               GetContactCenters() == other.GetContactCenters() &&
               GetIncomingCommuningCities() == other.GetIncomingCommuningCities() &&
               GetOutgoingCommuningCities() == other.GetOutgoingCommuningCities() &&
               ((!GetParent() && !other.GetParent()) ||
                (GetParent() && other.GetParent() && *GetParent() == *other.GetParent())) &&
               sub1.size() == sub2.size() &&
               std::equal(sub1.begin(), sub1.end(), sub1.begin(), sub1.end(),
                          [](std::shared_ptr<Location> lhs, std::shared_ptr<Location> rhs) {
                                  return lhs->GetID() == rhs->GetID();
                          });
}

void Location::CalculatePopulation(unsigned int totalPopulation)
{
        m_population = static_cast<unsigned int>(std::floor(m_relativePopulation * totalPopulation));
}
void   Location::SetRelativePopulation(double relativePopulation) { m_relativePopulation = relativePopulation; }
double Location::GetRelativePopulationSize() const { return m_relativePopulation; }

void Location::AddSubMunicipality(std::shared_ptr<Location> location)
{
        if (m_parent) {
                throw Exception("Can't have parent and submunicipalities at the same time!");
        }
        m_subMunicipalities.emplace(std::move(location));
}

const std::set<std::shared_ptr<Location>>& Location::GetSubMunicipalities() const { return m_subMunicipalities; }

std::shared_ptr<Location> Location::GetParent() const { return m_parent; }

void Location::SetParent(const std::shared_ptr<Location>& location)
{
        if (!m_subMunicipalities.empty()) {
                throw Exception("Can't have parent and submunicipalities at the same time!");
        }
        m_parent = location;
}

unsigned int Location::GetPopulationOfSubmunicipalities() const
{
        unsigned int total = 0;
        for (const auto& subMunicipality : m_subMunicipalities) {
                total += subMunicipality->GetPopulation();
        }
        return total;
}

} // namespace gengeopop
