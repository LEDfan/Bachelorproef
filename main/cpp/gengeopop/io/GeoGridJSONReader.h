#pragma once

#include "GeoGridReader.h"

namespace gengeopop {

class GeoGridJSONReader : public GeoGridReader
{
public:
        GeoGridJSONReader(std::unique_ptr<std::istream> inputStream);
        std::shared_ptr<GeoGrid> Read();

private:
        std::shared_ptr<Location>      ParseLocation(boost::property_tree::ptree& location);
        Coordinate                     ParseCoordinate(boost::property_tree::ptree& coordinate);
        std::shared_ptr<ContactCenter> ParseContactCenter(boost::property_tree::ptree& contactCenter);
        stride::ContactPool*           ParseContactPool(boost::property_tree::ptree& contactPool, unsigned int poolSize,
                                                        stride::ContactPoolType::Id typeId);
        stride::Person*                ParsePerson(boost::property_tree::ptree& person);
        std::shared_ptr<GeoGrid>       m_geoGrid;
};
} // namespace gengeopop
