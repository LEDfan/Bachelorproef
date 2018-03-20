#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <gengeopop/Community.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/Household.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <iostream>

#include "GeoGridJSONReader.h"

namespace gengeopop {

GeoGridJSONReader::GeoGridJSONReader() : m_people() {}

std::shared_ptr<GeoGrid> GeoGridJSONReader::read(std::istream& stream)
{
        boost::property_tree::ptree root;
        boost::property_tree::read_json(stream, root);
        auto geoGrid = std::make_shared<GeoGrid>();

        auto people = root.get_child("persons");
        for (auto it = people.begin(); it != people.end(); it++) {
                auto person               = ParsePerson(it->second.get_child(""));
                m_people[person->GetId()] = person;
        }

        auto locations = root.get_child("locations");
        for (auto it = locations.begin(); it != locations.end(); it++) {
                geoGrid->addLocation(ParseLocation(it->second.get_child("")));
        }
        m_people.clear();
        return geoGrid;
}

std::shared_ptr<Location> GeoGridJSONReader::ParseLocation(boost::property_tree::ptree& location)
{
        unsigned int id         = boost::lexical_cast<unsigned int>(location.get<std::string>("id"));
        std::string  name       = location.get<std::string>("name");
        unsigned int province   = boost::lexical_cast<unsigned int>(location.get<std::string>("province"));
        unsigned int population = boost::lexical_cast<unsigned int>(location.get<std::string>("population"));
        Coordinate   coordinate = ParseCoordinate(location.get_child("coordinate"));

        auto result = std::make_shared<Location>(id, province, population, coordinate, name);

        auto contactCenters = location.get_child("contactCenters");

        for (auto it = contactCenters.begin(); it != contactCenters.end(); it++) {
                result->addContactCenter(ParseContactCenter(it->second.get_child("")));
        }
        return result;
}

Coordinate GeoGridJSONReader::ParseCoordinate(boost::property_tree::ptree& coordinate)
{
        double x         = boost::lexical_cast<double>(coordinate.get<std::string>("x"));
        double y         = boost::lexical_cast<double>(coordinate.get<std::string>("y"));
        double longitude = boost::lexical_cast<double>(coordinate.get<std::string>("longitude"));
        double latitude  = boost::lexical_cast<double>(coordinate.get<std::string>("latitude"));
        return Coordinate(x, y, longitude, latitude);
}

std::shared_ptr<ContactCenter> GeoGridJSONReader::ParseContactCenter(boost::property_tree::ptree& contactCenter)
{
        std::string                    type = contactCenter.get<std::string>("type");
        std::shared_ptr<ContactCenter> result;
        if (type == "School") {
                result = std::make_shared<School>();
        } else if (type == "Community") {
                result = std::make_shared<Community>();
        } else if (type == "HighSchool") {
                result = std::make_shared<HighSchool>();
        } else if (type == "Household") {
                result = std::make_shared<Household>();
        } else if (type == "Workplace") {
                result = std::make_shared<Workplace>();
        } else {
                throw std::invalid_argument("No such ContactCenter type: " + type);
        }

        auto contactPools = contactCenter.get_child("pools");
        for (auto it = contactPools.begin(); it != contactPools.end(); it++) {
                result->addPool(ParseContactPool(it->second.get_child("")));
        }
        return result;
}

std::shared_ptr<ContactPool> GeoGridJSONReader::ParseContactPool(boost::property_tree::ptree& contactPool)
{
        unsigned int id     = boost::lexical_cast<unsigned int>(contactPool.get<std::string>("id"));
        auto         result = std::make_shared<ContactPool>(id);
        auto         people = contactPool.get_child("people");
        for (auto it = people.begin(); it != people.end(); it++) {
                unsigned int person_id = boost::lexical_cast<unsigned int>(it->second.get<std::string>(""));
                if (m_people.count(person_id) == 0) {
                        throw std::invalid_argument("No such person: " + person_id);
                }
                result->addMember(m_people[person_id]);
        }

        return result;
}

std::shared_ptr<stride::Person> GeoGridJSONReader::ParsePerson(boost::property_tree::ptree& person)
{
        unsigned int id          = boost::lexical_cast<unsigned int>(person.get<std::string>("id"));
        unsigned int age         = boost::lexical_cast<unsigned int>(person.get<std::string>("age"));
        std::string  gender      = person.get<std::string>("gender");
        unsigned int schoolId    = boost::lexical_cast<unsigned int>(person.get<std::string>("School"));
        unsigned int householdId = boost::lexical_cast<unsigned int>(person.get<std::string>("Household"));
        unsigned int workplaceId = boost::lexical_cast<unsigned int>(person.get<std::string>("Workplace"));
        unsigned int primaryCommunityId =
            boost::lexical_cast<unsigned int>(person.get<std::string>("PrimaryCommunity"));
        unsigned int secondaryCommunityId =
            boost::lexical_cast<unsigned int>(person.get<std::string>("SecondaryCommunity"));

        return std::make_shared<stride::Person>(id, age, householdId, schoolId, workplaceId, primaryCommunityId,
                                                secondaryCommunityId, 0, 0, 0, 0, 0);
}

} // namespace gengeopop
