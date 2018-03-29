#include "GeoGridJSONWriter.h"
#include <iostream>
#include <omp.h>

namespace gengeopop {

GeoGridJSONWriter::GeoGridJSONWriter() : m_persons_found() {}

void GeoGridJSONWriter::write(std::shared_ptr<gengeopop::GeoGrid> geoGrid, std::ostream& stream)
{
        boost::property_tree::ptree root;
        boost::property_tree::ptree locations;

#pragma omp parallel
#pragma omp single
        {
                for (const auto& location : *geoGrid) {
                        std::pair<std::string, boost::property_tree::ptree> child;
#pragma omp task firstprivate(location)
                        {
                                child = std::make_pair("", parseLocation(location));
#pragma omp critical

                                locations.push_back(std::move(child));
                        }
                }
#pragma omp taskwait
        }

        root.add_child("locations", locations);

        boost::property_tree::ptree persons;
#pragma omp parallel
#pragma omp single
        {
                for (const auto& person : m_persons_found) {
                        std::pair<std::string, boost::property_tree::ptree> child;
#pragma omp task firstprivate(person)
                        {
                                child = std::make_pair("", parsePerson(person));
#pragma omp critical
                                persons.push_back(std::move(child));
                        }
                }
#pragma omp taskwait
        }
        root.add_child("persons", persons);

        m_persons_found.clear();
        boost::property_tree::write_json(stream, root);
} // namespace gengeopop

boost::property_tree::ptree GeoGridJSONWriter::parseLocation(std::shared_ptr<Location> location)
{
        boost::property_tree::ptree location_root;
        location_root.put("id", location->getID());
        location_root.put("name", location->getName());
        location_root.put("province", location->getProvince());
        location_root.put("population", location->getPopulation());
        location_root.add_child("coordinate", parseCoordinate(location->getCoordinate()));

        boost::property_tree::ptree contactCenters;
#pragma omp parallel
#pragma omp single
        {
                for (const auto& contactCenter : *location) {
                        std::pair<std::string, boost::property_tree::ptree> child;
#pragma omp task firstprivate(contactCenter)
                        {
                                child = std::make_pair("", parseContactCenter(contactCenter));
#pragma omp critical
                                contactCenters.push_back(std::move(child));
                        }
                }
#pragma omp taskwait
        }
        location_root.add_child("contactCenters", contactCenters);
        return location_root;
}

boost::property_tree::ptree GeoGridJSONWriter::parseCoordinate(const Coordinate& coordinate)
{
        boost::property_tree::ptree coordinate_root;
        coordinate_root.put("x", coordinate.x);
        coordinate_root.put("y", coordinate.y);
        coordinate_root.put("longitude", coordinate.longitude);
        coordinate_root.put("latitude", coordinate.latitude);
        return coordinate_root;
}

boost::property_tree::ptree GeoGridJSONWriter::parseContactCenter(std::shared_ptr<ContactCenter> contactCenter)
{
        boost::property_tree::ptree contactCenter_root;
        contactCenter_root.put("type", contactCenter->getType());
        boost::property_tree::ptree pools;
#pragma omp parallel
#pragma omp single
        {
                for (const auto& pool : *contactCenter) {
                        std::pair<std::string, boost::property_tree::ptree> child;
#pragma omp task firstprivate(pool)
                        {
                                child = std::make_pair("", parseContactPool(pool));
#pragma omp critical
                                pools.push_back(std::move(child));
                        }
                }
#pragma omp taskwait
        }
        contactCenter_root.add_child("pools", pools);
        return contactCenter_root;
}

boost::property_tree::ptree GeoGridJSONWriter::parseContactPool(std::shared_ptr<ContactPool> contactPool)
{
        boost::property_tree::ptree contactPool_root;
        contactPool_root.put("id", contactPool->getID());
        boost::property_tree::ptree people;
        for (const auto& person : *contactPool) {
                boost::property_tree::ptree person_root;
#pragma omp critical
                m_persons_found.insert(person);
                person_root.put("", person->GetId());
                people.push_back(std::make_pair("", person_root));
        }
        contactPool_root.add_child("people", people);
        return contactPool_root;
}
boost::property_tree::ptree GeoGridJSONWriter::parsePerson(std::shared_ptr<stride::Person> person)
{
        boost::property_tree::ptree person_root;
        person_root.put("id", person->GetId());
        person_root.put("age", person->GetAge());
        person_root.put("gender", person->GetGender());
        person_root.put("School", person->GetSchoolId());
        person_root.put("Household", person->GetHouseholdId());
        person_root.put("Workplace", person->GetWorkId());
        person_root.put("PrimaryCommunity", person->GetPrimaryCommunityId());
        person_root.put("SecondaryCommunity", person->GetSecondaryCommunityId());
        return person_root;
}
} // namespace gengeopop
