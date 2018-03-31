#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <gengeopop/Community.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/Household.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <iostream>
#include <memory>
#include <mutex>

#include "GeoGridJSONReader.h"

namespace {

// Based on https://stackoverflow.com/a/13978507/1393103
class ThreadException
{
public:
        ThreadException() : ptr(nullptr), Lock() {}
        ~ThreadException() {}

        void Rethrow()
        {
                if (this->ptr != nullptr) {
                        std::rethrow_exception(this->ptr);
                }
        }
        void CaptureException()
        {
                std::unique_lock<std::mutex> guard(this->Lock);
                this->ptr = std::current_exception();
        }

        template <typename Function, typename... Parameters>
        void Run(Function f, Parameters... params)
        {
                try {
                        f(params...);
                } catch (...) {
                        CaptureException();
                }
        }

        bool HasError() const { return ptr != nullptr; }

private:
        std::exception_ptr ptr;
        std::mutex         Lock;
};
} // namespace

namespace gengeopop {

GeoGridJSONReader::GeoGridJSONReader() : m_people(), m_commutes() {}

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
#pragma omp parallel
#pragma omp single
        {
                for (auto it = people.begin(); it != people.end(); it++) {
                        std::shared_ptr<stride::Person> person;
#pragma omp task firstprivate(it, person)
                        {
                                person = ParsePerson(it->second.get_child(""));
#pragma omp critical
                                m_people[person->GetId()] = std::move(person);
                        }
                }
#pragma omp taskwait
        }
        auto locations = root.get_child("locations");
        auto e         = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (auto it = locations.begin(); it != locations.end(); it++) {
                        std::shared_ptr<Location> loc;
#pragma omp task firstprivate(it, loc)
                        {
                                e->Run([&loc, this, &it] { loc = ParseLocation(it->second.get_child("")); });
                                if (!e->HasError())
#pragma omp critical
                                        geoGrid->addLocation(std::move(loc));
                        }
                }
#pragma omp taskwait
        }

        for (const auto& commute_tuple : m_commutes) {
                auto a      = geoGrid->GetById(std::get<0>(commute_tuple));
                auto b      = geoGrid->GetById(std::get<1>(commute_tuple));
                auto amount = std::get<2>(commute_tuple);
                a->addOutgoingCommutingLocation(b, amount);
                b->addIncomingCommutingLocation(a, amount);
        }

        e->Rethrow();
        m_people.clear();
        m_commutes.clear();
        return geoGrid;
} // namespace gengeopop

std::shared_ptr<Location> GeoGridJSONReader::ParseLocation(boost::property_tree::ptree& location)
{
        auto        id         = boost::lexical_cast<unsigned int>(location.get<std::string>("id"));
        std::string name       = location.get<std::string>("name");
        auto        province   = boost::lexical_cast<unsigned int>(location.get<std::string>("province"));
        auto        population = boost::lexical_cast<unsigned int>(location.get<std::string>("population"));
        Coordinate  coordinate = ParseCoordinate(location.get_child("coordinate"));

        auto result = std::make_shared<Location>(id, province, population, coordinate, name);

        auto contactCenters = location.get_child("contactCenters");

        auto e = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (auto it = contactCenters.begin(); it != contactCenters.end(); it++) {
                        std::shared_ptr<ContactCenter> center;
#pragma omp task firstprivate(it, center)
                        {
                                e->Run([&it, this, &center] { center = ParseContactCenter(it->second.get_child("")); });
                                if (!e->HasError())
#pragma omp critical
                                        result->addContactCenter(center);
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();

        if (location.count("commutes")) {
                boost::property_tree::ptree commutes = location.get_child("commutes");
                for (auto it = commutes.begin(); it != commutes.end(); it++) {
                        auto to     = boost::lexical_cast<unsigned int>(it->first);
                        auto amount = boost::lexical_cast<double>(it->second.data());
#pragma omp critical
                        m_commutes.push_back(std::make_tuple(id, to, amount));
                }
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
        auto                           id = boost::lexical_cast<unsigned int>(contactCenter.get<std::string>("id"));
        if (type == "School") {
                result = std::make_shared<School>(id);
        } else if (type == "Community") {
                result = std::make_shared<Community>(id);
        } else if (type == "HighSchool") {
                result = std::make_shared<HighSchool>(id);
        } else if (type == "Household") {
                result = std::make_shared<Household>(id);
        } else if (type == "Workplace") {
                result = std::make_shared<Workplace>(id);
        } else {
                throw std::invalid_argument("No such ContactCenter type: " + type);
        }

        auto contactPools = contactCenter.get_child("pools");

        auto e = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (auto it = contactPools.begin(); it != contactPools.end(); it++) {
                        std::shared_ptr<ContactPool> pool;
#pragma omp task firstprivate(it, pool)
                        {
                                e->Run([&it, &pool, this, &result] {
                                        pool = ParseContactPool(it->second.get_child(""), result->getPoolSize());
                                });
                                if (!e->HasError())
#pragma omp critical
                                        result->addPool(pool);
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();
        return result;
}

std::shared_ptr<ContactPool> GeoGridJSONReader::ParseContactPool(boost::property_tree::ptree& contactPool,
                                                                 unsigned int                 poolSize)
{
        unsigned int id     = boost::lexical_cast<unsigned int>(contactPool.get<std::string>("id"));
        auto         result = std::make_shared<ContactPool>(id, poolSize);
        auto         people = contactPool.get_child("people");

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
