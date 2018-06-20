#pragma once

#include "GeoGridReader.h"

namespace gengeopop {

/**
 * An implementation of the GeoGridReader using JSON
 * This class is used to read a GeoGrid from a JSON file
 */

class GeoGridJSONReader : public GeoGridReader
{
public:
        /// Construct the GeoGridJSONReader with the istream which contains the JSON
        GeoGridJSONReader(std::unique_ptr<std::istream> inputStream);

        /// Actually perform the read and return the GeoGrid
        std::shared_ptr<GeoGrid> Read();

private:
        /// Create a Location based on the information stored in the provided boost property tree
        std::shared_ptr<Location> ParseLocation(boost::property_tree::ptree& location);

        /// Create a Coordinate based on the information stored in the provided boost property tree
        Coordinate ParseCoordinate(boost::property_tree::ptree& coordinate);

        /// Create a ContactCenter based on the information stored in the provided boost property tree
        std::shared_ptr<ContactCenter> ParseContactCenter(boost::property_tree::ptree& contactCenter);

        /// Create a ContactCenter based on the information stored in the provided boost property tree
        stride::ContactPool* ParseContactPool(boost::property_tree::ptree& contactPool,
                                              stride::ContactPoolType::Id  typeId);

        /// Create a Person based on the information stored in the provided boost property tree
        stride::Person* ParsePerson(boost::property_tree::ptree& person);

        std::shared_ptr<GeoGrid> m_geoGrid; ///< The GeoGrid which is being built
};
} // namespace gengeopop
