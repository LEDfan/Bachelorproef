#include "GeoGridProtoReader.h"
#include "ThreadException.h"
#include "proto/geogrid.pb.h"
#include <gengeopop/Community.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/Household.h>
#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/School.h>
#include <gengeopop/SecondaryCommunity.h>
#include <gengeopop/Workplace.h>
#include <iostream>
#include <omp.h>

namespace gengeopop {

GeoGridProtoReader::GeoGridProtoReader() : m_people(), m_commutes() {}

std::shared_ptr<GeoGrid> GeoGridProtoReader::read(std::istream& stream)
{
        proto::GeoGrid protoGrid;
        if (!protoGrid.ParseFromIstream(&stream)) {
                throw std::runtime_error("Failed to parse Proto file");
        }
        auto geoGrid = std::make_shared<GeoGrid>();
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoGrid.persons_size(); idx++) {
                        const proto::GeoGrid_Person& protoPerson = protoGrid.persons(idx);
#pragma omp task firstprivate(protoPerson)
                        {
                                const auto& person = ParsePerson(protoPerson);
#pragma omp critical
                                m_people[person->GetId()] = person;
                        }
                }
#pragma omp taskwait
        }
        auto e = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoGrid.locations_size(); idx++) {
                        const proto::GeoGrid_Location& protoLocation = protoGrid.locations(idx);
#pragma omp task firstprivate(protoLocation)
                        {
                                std::shared_ptr<Location> loc =
                                    *(e->Run([this, &protoLocation]() -> std::shared_ptr<Location> {
                                            return ParseLocation(protoLocation);
                                    }));
                                if (!e->HasError())
#pragma omp critical
                                        geoGrid->addLocation(std::move(loc));
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();

        for (const auto& commute_tuple : m_commutes) {
                const auto& a      = geoGrid->GetById(std::get<0>(commute_tuple));
                const auto& b      = geoGrid->GetById(std::get<1>(commute_tuple));
                auto        amount = std::get<2>(commute_tuple);
                a->addOutgoingCommutingLocation(b, amount);
                b->addIncomingCommutingLocation(a, amount);
        }

        m_people.clear();
        m_commutes.clear();
        return geoGrid;
}

std::shared_ptr<Location> GeoGridProtoReader::ParseLocation(const proto::GeoGrid_Location& protoLocation)
{
        auto               id         = protoLocation.id();
        const std::string& name       = protoLocation.name();
        auto               province   = protoLocation.province();
        auto               population = protoLocation.population();
        const Coordinate&  coordinate = ParseCoordinate(protoLocation.coordinate());

        auto result = std::make_shared<Location>(id, province, population, coordinate, name);

        auto e = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoLocation.contactcenters_size(); idx++) {
                        const proto::GeoGrid_Location_ContactCenter& protoCenter = protoLocation.contactcenters(idx);
#pragma omp task firstprivate(protoCenter)
                        {
                                std::shared_ptr<ContactCenter> center =
                                    *(e->Run([&protoCenter, this] { return ParseContactCenter(protoCenter); }));
                                if (!e->HasError())
#pragma omp critical
                                        result->addContactCenter(center);
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();

        const auto& s = protoLocation.commutes_size();
#pragma omp critical
        m_commutes.reserve(m_commutes.size() + s);
        for (int idx = 0; idx < s; idx++) {
                const proto::GeoGrid_Location_Commute& commute = protoLocation.commutes(idx);
#pragma omp critical
                m_commutes.push_back(std::make_tuple(id, commute.to(), commute.proportion()));
        }

        return result;
}

Coordinate GeoGridProtoReader::ParseCoordinate(const proto::GeoGrid_Location_Coordinate& protoCoordinate)
{
        double x         = protoCoordinate.x();
        double y         = protoCoordinate.y();
        double longitude = protoCoordinate.longitude();
        double latitude  = protoCoordinate.latitude();
        return {x, y, longitude, latitude};
}

std::shared_ptr<ContactCenter> GeoGridProtoReader::ParseContactCenter(
    const proto::GeoGrid_Location_ContactCenter& protoContactCenter)
{
        const std::string&             type = protoContactCenter.type();
        std::shared_ptr<ContactCenter> result;
        auto                           id = protoContactCenter.id();

        if (type == "School") {
                result = std::make_shared<School>(id);
        } else if (type == "Community") {
                result = std::make_shared<Community>(id);
        } else if (type == "HighSchool") {
                result = std::make_shared<HighSchool>(id);
        } else if (type == "Household") {
                result = std::make_shared<Household>(id);
        } else if (type == "Primary Community") {
                result = std::make_shared<PrimaryCommunity>(id);
        } else if (type == "Secondary Community") {
                result = std::make_shared<SecondaryCommunity>(id);
        } else if (type == "Workplace") {
                result = std::make_shared<Workplace>(id);
        } else {
                throw std::invalid_argument("No such ContactCenter type: " + type);
        }

        const auto& poolsize = result->getPoolSize();
        auto        e        = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoContactCenter.pools_size(); idx++) {
                        const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool =
                            protoContactCenter.pools(idx);
#pragma omp task firstprivate(protoContactPool)
                        {
                                std::shared_ptr<ContactPool> pool =
                                    *(e->Run([&protoContactPool, this, &poolsize ]() -> auto {
                                            return ParseContactPool(protoContactPool, poolsize);
                                    }));
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

std::shared_ptr<ContactPool> GeoGridProtoReader::ParseContactPool(
    const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool, unsigned int poolSize)
{
        const unsigned int& id     = static_cast<const unsigned int&>(protoContactPool.id());
        auto                result = std::make_shared<ContactPool>(id, poolSize);

        for (int idx = 0; idx < protoContactPool.people_size(); idx++) {
                const auto& person_id = protoContactPool.people(idx);
                result->addMember(m_people.at(static_cast<unsigned int>(person_id)));
        }

        return result;
}

std::shared_ptr<stride::Person> GeoGridProtoReader::ParsePerson(const proto::GeoGrid_Person& protoPerson)
{
        auto               id                   = protoPerson.id();
        auto               age                  = protoPerson.age();
        const std::string& gender               = protoPerson.gender();
        auto               schoolId             = protoPerson.school();
        auto               householdId          = protoPerson.household();
        auto               workplaceId          = protoPerson.workplace();
        auto               primaryCommunityId   = protoPerson.primarycommunity();
        auto               secondaryCommunityId = protoPerson.secondarycommunity();

        return std::make_shared<stride::Person>(id, age, householdId, schoolId, workplaceId, primaryCommunityId,
                                                secondaryCommunityId, 0, 0, 0, 0, 0);
}

} // namespace gengeopop
