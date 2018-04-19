#pragma once

#include "GeoGridReader.h"

namespace gengeopop {

class GeoGridJSONReader : public GeoGridReader
{
public:
        GeoGridJSONReader(std::unique_ptr<std::istream> inputStream);
        std::shared_ptr<GeoGrid> read();

private:
        std::shared_ptr<Location>      ParseLocation(boost::property_tree::ptree& location);
        Coordinate                     ParseCoordinate(boost::property_tree::ptree& coordinate);
        std::shared_ptr<ContactCenter> ParseContactCenter(boost::property_tree::ptree& contactCenter);
        std::shared_ptr<ContactPool> ParseContactPool(boost::property_tree::ptree& contactPool, unsigned int poolSize);
        std::shared_ptr<stride::Person> ParsePerson(boost::property_tree::ptree& person);
};
} // namespace gengeopop
