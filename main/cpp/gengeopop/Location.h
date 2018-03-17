#pragma once

#include "ContactCenter.h"
#include "Coordinate.h"
#include <string>
#include <vector>

namespace gengeopop {
class Location
{
public:
        using iterator = std::vector<std::shared_ptr<ContactCenter>>::iterator;

        Location(unsigned int id, unsigned int province, unsigned int population = 0,
                 Coordinate coordinate = Coordinate(0.0, 0.0, 0.0, 0.0), std::string name = "");

        void         addContactCenter(std::shared_ptr<ContactCenter> center);
        std::string  getName() const;
        unsigned int getProvince() const;
        unsigned int getID() const;

        unsigned int getPopulation() const;

        /**
         *
         * @return a vector with the outgoing cities which people are commuting to + the proportion
         */
        const std::vector<std::pair<std::shared_ptr<Location>, double>>& getIncomingCommuningCities() const;

        /**
         * Adds a Location and a proportion to the incoming commutng vector
         * I.e. $proportion of the commuting population in $location are commuting to $this
         */
        void addIncomingCommutingLocation(std::shared_ptr<Location> location, double proportion);

        /**
         *
         * @return a vector with the outgoing cities which people are commuting to + the proportion
         */
        const std::vector<std::pair<std::shared_ptr<Location>, double>>& getOutgoingCommuningCities() const;

        /**
         * Adds a Location and a proportion to the incoming commuting vector
         * I.e. $proportion of the commuting population in $this are commuting to $location
         */
        void addOutgoingCommutingLocation(std::shared_ptr<Location> location, double proportion);

        int outGoingCommutingPeople(double fractionOfPopulationCommuting) const;
        int incomingCommutingPeople(double fractionOfPopulationCommuting) const;

        const Coordinate& getCoordinate() const;

        iterator begin();
        iterator end();

        const std::vector<std::shared_ptr<ContactCenter>>& getContactCenters() const;

private:
        unsigned int                                              m_id = 0;
        std::string                                               m_name;
        unsigned int                                              m_province;
        unsigned int                                              m_population;
        Coordinate                                                m_coordinate;
        std::vector<std::shared_ptr<ContactCenter>>               m_contactCenters;
        std::vector<std::pair<std::shared_ptr<Location>, double>> m_incomingCommutingLocations;
        std::vector<std::pair<std::shared_ptr<Location>, double>> m_outgoingCommutingLocations;
};

} // namespace gengeopop
