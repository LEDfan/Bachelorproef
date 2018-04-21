#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <set>

#include "GeoGridWriter.h"

namespace gengeopop {

class GeoGridJSONWriter : public GeoGridWriter
{
public:
        GeoGridJSONWriter();

        void write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream);

private:
        boost::property_tree::ptree writeLocation(std::shared_ptr<Location> location);
        boost::property_tree::ptree writeCoordinate(const Coordinate& coordinate);
        boost::property_tree::ptree writeContactCenter(std::shared_ptr<ContactCenter> contactCenter);
        boost::property_tree::ptree writeContactPool(std::shared_ptr<ContactPool> contactPool);
        boost::property_tree::ptree writePerson(stride::Person* person);

        std::set<stride::Person*> m_persons_found;
};
} // namespace gengeopop
