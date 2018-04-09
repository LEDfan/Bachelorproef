#pragma once

#include "ContactCenter.h"
#include "Coordinate.h"
#include <iostream>
#include <set>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace gengeopop {
class Location
{
public:
        using iterator = std::vector<std::shared_ptr<ContactCenter>>::iterator;

        Location(unsigned int id, unsigned int province, Coordinate coordinate = Coordinate(0.0, 0.0, 0.0, 0.0),
                 std::string name = "");
        Location(unsigned int id, unsigned int province, unsigned int population,
                 Coordinate coordinate = Coordinate(0.0, 0.0, 0.0, 0.0), std::string name = "");

        template <typename T>
        void addContactCenter(const std::shared_ptr<T>& contactCenter)
        {
                m_contactCenters.push_back(contactCenter);
                m_contactCenterByType[typeid(T)].insert(contactCenter);
        }

        template <typename T>
        std::set<std::shared_ptr<ContactCenter>> getContactCentersOfType()
        {
                return m_contactCenterByType[typeid(T)];
        }

        std::string  getName() const;
        unsigned int getProvince() const;
        unsigned int getID() const;

        unsigned int getPopulation() const;

        /**
         * Given the total population calculates this location's population using the relative population of this
         * location.
         */
        void calculatePopulation(unsigned int totalPopulation);

        void setRelativePopulation(double relativePopulation);

        double getRelativePopulationSize() const;

        /**
         *
         * @return a vector with the outgoing cities which people are commuting to + the proportion
         */
        const std::vector<std::pair<std::shared_ptr<Location>, double>>& getIncomingCommuningCities() const;

        /**
         * Adds a Location and a proportion to the incoming commutng vector
         * I.e. \p proportion of the commuting population in \p location are commuting to \p this
         */
        void addIncomingCommutingLocation(std::shared_ptr<Location> location, double proportion);

        /**
         *
         * @return a vector with the outgoing cities which people are commuting to + the proportion
         */
        const std::vector<std::pair<std::shared_ptr<Location>, double>>& getOutgoingCommuningCities() const;

        /**
         * Adds a Location and a proportion to the incoming commuting vector
         * I.e. \p proportion of the commuting population in \p this are commuting to \p location
         */
        void addOutgoingCommutingLocation(std::shared_ptr<Location> location, double proportion);

        int outGoingCommutingPeople(double fractionOfPopulationCommuting) const;
        int incomingCommutingPeople(double fractionOfPopulationCommuting) const;

        const Coordinate& getCoordinate() const;

        iterator begin();
        iterator end();

        const std::vector<std::shared_ptr<ContactCenter>>& getContactCenters() const;

        bool operator==(const Location& other) const;

private:
        unsigned int                                                                  m_id = 0;
        std::string                                                                   m_name;
        unsigned int                                                                  m_province;
        unsigned int                                                                  m_population;
        double                                                                        m_relativePopulation;
        Coordinate                                                                    m_coordinate;
        std::vector<std::shared_ptr<ContactCenter>>                                   m_contactCenters;
        std::vector<std::pair<std::shared_ptr<Location>, double>>                     m_incomingCommutingLocations;
        std::vector<std::pair<std::shared_ptr<Location>, double>>                     m_outgoingCommutingLocations;
        std::unordered_map<std::type_index, std::set<std::shared_ptr<ContactCenter>>> m_contactCenterByType;
};

} // namespace gengeopop
