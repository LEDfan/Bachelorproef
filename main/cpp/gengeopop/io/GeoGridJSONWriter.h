#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <set>

#include "GeoGridWriter.h"

namespace gengeopop {

/**
 * An implementation of the GeoGridWriter using JSON
 * This class is used to write a GeoGrid to a JSON file
 */
class GeoGridJSONWriter : public GeoGridWriter
{
public:
        /// Construct the GeoGridJSONWriter
        GeoGridJSONWriter();

        /// Write the provided GeoGrid to the proved ostream in JSON format
        void Write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream) override;

private:
        /// Create a Boost Property Tree containing all the information needed to reconstruct a Location
        boost::property_tree::ptree WriteLocation(std::shared_ptr<Location> location);

        /// Create a Boost Property Tree containing all the information needed to reconstruct a Coordinate
        boost::property_tree::ptree WriteCoordinate(const Coordinate& coordinate);

        /// Create a Boost Property Tree containing all the information needed to reconstruct a ContactCenter
        boost::property_tree::ptree WriteContactCenter(std::shared_ptr<ContactCenter> contactCenter);

        /// Create a Boost Property Tree containing all the information needed to reconstruct a ContactPool
        boost::property_tree::ptree WriteContactPool(stride::ContactPool* contactPool);

        /// Create a Boost Property Tree containing all the information needed to reconstruct a Person
        boost::property_tree::ptree WritePerson(stride::Person* person);

        std::set<stride::Person*>
            m_persons_found; ///< The persons found when looping over the ContactPools.
                             ///< These are needed since they are stored in a separate list inside the JSON file.
};
} // namespace gengeopop
