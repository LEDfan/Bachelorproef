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

        void Write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream) override;

private:
        boost::property_tree::ptree WriteLocation(std::shared_ptr<Location> location);
        boost::property_tree::ptree WriteCoordinate(const Coordinate& coordinate);
        boost::property_tree::ptree WriteContactCenter(std::shared_ptr<ContactCenter> contactCenter);
        boost::property_tree::ptree WriteContactPool(stride::ContactPool* contactPool);
        boost::property_tree::ptree WritePerson(stride::Person* person);

        std::set<stride::Person*> m_persons_found;
};
} // namespace gengeopop
