#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <gengeopop/Community.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/Household.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <iostream>
#include <memory>

#include "GeoGridJSONReader.h"

namespace gengeopop {

GeoGridJSONReader::GeoGridJSONReader() : m_people() {}

std::shared_ptr<GeoGrid> GeoGridJSONReader::read(std::istream& stream)
{
        boost::property_tree::ptree root;
        try {
                boost::property_tree::read_json(stream, root);
        } catch (std::runtime_error) {
                throw std::runtime_error(
                    "There was a problem parsing the JSON file, please check if it is not empty and it is valid JSON.");
        }
        auto geoGrid = std::make_shared<GeoGrid>();
        auto people  = root.get_child("persons");
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
        auto        id         = boost::lexical_cast<unsigned int>(location.get<std::string>("id"));
        std::string name       = location.get<std::string>("name");
        auto        province   = boost::lexical_cast<unsigned int>(location.get<std::string>("province"));
        auto        population = boost::lexical_cast<unsigned int>(location.get<std::string>("population"));
        Coordinate  coordinate = ParseCoordinate(location.get_child("coordinate"));

        auto result = std::make_shared<Location>(id, province, population, coordinate, name);

        auto contactCenters = location.get_child("contactCenters");

        for (auto it = contactCenters.begin(); it != contactCenters.end(); it++) {
                result->addContactCenter(ParseContactCenter(it->second.get_child("")));
        }
        return result;
}

Coordinate GeoGridJSONReader::ParseCoordinate(boost::property_tree::ptree& coordinate)
{
        auto x         = boost::lexical_cast<double>(coordinate.get<std::string>("x"));
        auto y         = boost::lexical_cast<double>(coordinate.get<std::string>("y"));
        auto longitude = boost::lexical_cast<double>(coordinate.get<std::string>("longitude"));
        auto latitude  = boost::lexical_cast<double>(coordinate.get<std::string>("latitude"));
        return {x, y, longitude, latitude};
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
        auto id     = boost::lexical_cast<unsigned int>(contactPool.get<std::string>("id"));
        auto result = std::make_shared<ContactPool>(id);
        auto people = contactPool.get_child("people");
        for (auto it = people.begin(); it != people.end(); it++) {
                auto person_id = boost::lexical_cast<unsigned int>(it->second.get<std::string>(""));
                if (m_people.count(person_id) == 0) {
                        throw std::invalid_argument("No such person: " + std::to_string(person_id));
                }
                result->addMember(m_people[person_id]);
        }

        return result;
}

std::shared_ptr<stride::Person> GeoGridJSONReader::ParsePerson(boost::property_tree::ptree& person)
{
        auto        id                 = boost::lexical_cast<unsigned int>(person.get<std::string>("id"));
        auto        age                = boost::lexical_cast<unsigned int>(person.get<std::string>("age"));
        std::string gender             = person.get<std::string>("gender");
        auto        schoolId           = boost::lexical_cast<unsigned int>(person.get<std::string>("School"));
        auto        householdId        = boost::lexical_cast<unsigned int>(person.get<std::string>("Household"));
        auto        workplaceId        = boost::lexical_cast<unsigned int>(person.get<std::string>("Workplace"));
        auto        primaryCommunityId = boost::lexical_cast<unsigned int>(person.get<std::string>("PrimaryCommunity"));
        auto secondaryCommunityId = boost::lexical_cast<unsigned int>(person.get<std::string>("SecondaryCommunity"));

        return std::make_shared<stride::Person>(id, age, householdId, schoolId, workplaceId, primaryCommunityId,
                                                secondaryCommunityId, 0, 0, 0, 0, 0);
}

} // namespace gengeopop