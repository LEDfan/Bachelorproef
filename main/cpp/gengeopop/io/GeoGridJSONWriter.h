#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <set>

#include "GeoGridWriter.h"

namespace gengeopop {

    class GeoGridJSONWriter: public GeoGridWriter{
    public:
            GeoGridJSONWriter();

        void write(std::shared_ptr<GeoGrid> geoGrid, std::ostream &stream);

    private:
        boost::property_tree::ptree parseLocation(std::shared_ptr<Location> location);
        boost::property_tree::ptree parseCoordinate(const Coordinate& coordinate);
        boost::property_tree::ptree parseContactCenter(std::shared_ptr<ContactCenter> contactCenter);
        boost::property_tree::ptree parseContactPool(std::shared_ptr<ContactPool> contactPool);
        boost::property_tree::ptree parsePerson(std::shared_ptr<stride::Person> person);

        std::set<std::shared_ptr<stride::Person>> m_persons_found;
    };
}


