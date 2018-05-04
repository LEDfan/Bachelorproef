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

        /// Add a ContactCenter
        template <typename T>
        void AddContactCenter(std::shared_ptr<T> contactCenter)
        {
                m_contactCenters.push_back(contactCenter);
                m_contactCenterByType[typeid(T)].insert(contactCenter);
        }

        /// Gets the Contact Centers of a specific type \p T
        template <typename T>
        std::set<std::shared_ptr<ContactCenter>> GetContactCentersOfType()
        {
                return m_contactCenterByType[typeid(T)];
        }

        /// Gets the name
        std::string GetName() const;

        /// Gets the province
        unsigned int GetProvince() const;

        /// Gets the ID
        unsigned int GetID() const;

        /// Gets the absolute population
        unsigned int GetPopulation() const;

        /// Given the total population calculates this location's population using the relative population of this
        /// location
        void CalculatePopulation(unsigned int totalPopulation);

        /// Sets the relative population, which will be later used by @see CalculatePopulation
        void SetRelativePopulation(double relativePopulation);

        /// Gets the relative population
        double GetRelativePopulationSize() const;

        //// Gets the sum of the population of all sub municipalities
        unsigned int GetPopulationOfSubmunicipalities() const;

        /// Gets a vector with the outgoing cities which people are commuting to + the proportion
        const std::vector<std::pair<std::shared_ptr<Location>, double>>& GetIncomingCommuningCities() const;

        /**
         * Adds a Location and a proportion to the incoming commutng vector
         * I.e. \p proportion of the commuting population in \p location are commuting to \p this
         */
        void AddIncomingCommutingLocation(std::shared_ptr<Location> location, double proportion);

        /// @return a vector with the outgoing cities which people are commuting to + the proportion
        const std::vector<std::pair<std::shared_ptr<Location>, double>>& GetOutgoingCommuningCities() const;

        /**
         * Adds a Location and a proportion to the incoming commuting vector
         * I.e. \p proportion of the commuting population in \p this are commuting to \p location
         */
        void AddOutgoingCommutingLocation(std::shared_ptr<Location> location, double proportion);

        /// Gets the absolute amount of poeple leaving from this location
        int OutGoingCommutingPeople(double fractionOfPopulationCommuting) const;

        /// Gets the absolute amount of people going to this location
        int IncomingCommutingPeople(double fractionOfPopulationCommuting) const;

        /// Add a sub-municipality to this location
        void AddSubMunicipality(std::shared_ptr<Location> location);

        /// Gets the sub-municipalities of this location
        const std::set<std::shared_ptr<Location>>& GetSubMunicipalities() const;

        /// Sets the parent of this sub-municipality, requires that this location doesn't have any sub-municipality
        void SetParent(const std::shared_ptr<Location>& location);

        /// Gets the parent of this sub-municipality
        std::shared_ptr<Location> GetParent() const;

        /// Gets the Coordinate of this location
        const Coordinate& GetCoordinate() const;

        /// Gets iterator to the first location
        iterator begin();

        // Gets iterator to the end of the locations
        iterator end();

        /// Gets all contact centers at this location
        const std::vector<std::shared_ptr<ContactCenter>>& GetContactCenters() const;

        /// Perform a full compare of the given locations.
        bool operator==(const Location& other) const;

private:
        unsigned int m_id = 0;             ///< Id
        std::string  m_name;               ///< Name
        unsigned int m_province;           ///< Province id
        unsigned int m_population;         ///< The absolute population
        double       m_relativePopulation; ///< The relative population (relative against whole population)
        Coordinate   m_coordinate;         ///< Coordinate
        std::vector<std::shared_ptr<ContactCenter>>               m_contactCenters;             ///< All contactCenters
        std::vector<std::pair<std::shared_ptr<Location>, double>> m_incomingCommutingLocations; ///< Incomming commutes
                                                                                                ///< stored as pair of
                                                                                                ///< location and
                                                                                                ///< proportion relative
                                                                                                ///< to the given
                                                                                                ///< location
        std::vector<std::pair<std::shared_ptr<Location>, double>> m_outgoingCommutingLocations; ///< Outgoing commutes
                                                                                                ///< stored as pair of
                                                                                                ///< location and
                                                                                                ///< proportion relative
                                                                                                ///< to the this
                                                                                                ///< location
        std::set<std::shared_ptr<Location>>
            m_subMunicipalities; ///< Set of sub-municipalties, must be empty when m_parent is set
        std::shared_ptr<Location>
            m_parent; ///< Parent of this sub-municipality, must be nullptr when m_subMunicipalities is et
        std::unordered_map<std::type_index, std::set<std::shared_ptr<ContactCenter>>>
            m_contactCenterByType; ///< Stores the contact centers indexed by their type
};

} // namespace gengeopop
