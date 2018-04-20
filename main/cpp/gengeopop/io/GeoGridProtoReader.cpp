#include "GeoGridProtoReader.h"
#include "ThreadException.h"
#include "proto/geogrid.pb.h"
#include <Exception.h>
#include <gengeopop/Community.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/Household.h>
#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/School.h>
#include <gengeopop/SecondaryCommunity.h>
#include <gengeopop/Workplace.h>
#include <iostream>
#include <omp.h>
#include <stdexcept>

namespace gengeopop {

GeoGridProtoReader::GeoGridProtoReader(std::unique_ptr<std::istream> inputStream)
    : GeoGridReader(std::move(inputStream))
{
}

std::shared_ptr<GeoGrid> GeoGridProtoReader::read()
{
        proto::GeoGrid protoGrid;
        if (!protoGrid.ParseFromIstream(m_inputStream.get())) {
                throw Exception("Failed to parse Proto file");
        }
        std::shared_ptr<GeoGrid> geoGrid;
        if (m_population) {
                geoGrid = std::make_shared<GeoGrid>(m_population);
        } else {
                geoGrid = std::make_shared<GeoGrid>();
        }
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoGrid.persons_size(); idx++) {
                        stride::Person*              person;
                        const proto::GeoGrid_Person& protoPerson = protoGrid.persons(idx);
#pragma omp task firstprivate(protoPerson, person)
                        {
#pragma omp critical
                                {
                                        person                    = ParsePerson(geoGrid, protoPerson);
                                        m_people[person->GetId()] = person;
                                }
                        }
                }
#pragma omp taskwait
        }
        auto e = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoGrid.locations_size(); idx++) {
                        std::shared_ptr<Location>      loc;
                        const proto::GeoGrid_Location& protoLocation = protoGrid.locations(idx);
#pragma omp task firstprivate(protoLocation, loc)
                        {
                                e->Run([&loc, this, &protoLocation] { loc = ParseLocation(protoLocation); });
                                if (!e->HasError())
#pragma omp critical
                                        geoGrid->addLocation(std::move(loc));
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();
        addCommutes(geoGrid);
        addSubMunicipalities(geoGrid);
        m_people.clear();
        m_commutes.clear();
        m_subMunicipalities.clear();
        return geoGrid;
} // namespace gengeopop

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
                        std::shared_ptr<ContactCenter>               center;
                        const proto::GeoGrid_Location_ContactCenter& protoCenter = protoLocation.contactcenters(idx);
#pragma omp task firstprivate(protoCenter, center)
                        {
                                e->Run([&protoCenter, this, &center] { center = ParseContactCenter(protoCenter); });
                                if (!e->HasError())
#pragma omp critical
                                        result->addContactCenter(center);
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();

        for (int idx = 0; idx < protoLocation.commutes_size(); idx++) {
                const proto::GeoGrid_Location_Commute& commute = protoLocation.commutes(idx);
#pragma omp critical
                m_commutes.emplace_back(std::make_tuple(id, commute.to(), commute.proportion()));
        }

        for (int idx = 0; idx < protoLocation.submunicipalities_size(); idx++) {
                m_subMunicipalities.emplace_back(std::make_pair(result->getID(), protoLocation.submunicipalities(idx)));
        }
        return result;
} // namespace gengeopop

Coordinate GeoGridProtoReader::ParseCoordinate(const proto::GeoGrid_Location_Coordinate& protoCoordinate)
{
        const double x         = protoCoordinate.x();
        const double y         = protoCoordinate.y();
        const double longitude = protoCoordinate.longitude();
        const double latitude  = protoCoordinate.latitude();
        return {x, y, longitude, latitude};
}

std::shared_ptr<ContactCenter> GeoGridProtoReader::ParseContactCenter(
    const proto::GeoGrid_Location_ContactCenter& protoContactCenter)
{
        proto::GeoGrid_Location_ContactCenter_Type type = protoContactCenter.type();
        std::shared_ptr<ContactCenter>             result;
        auto                                       id = protoContactCenter.id();
        switch (type) {
        case proto::GeoGrid_Location_ContactCenter_Type_School: result = std::make_shared<School>(id); break;
        case proto::GeoGrid_Location_ContactCenter_Type_Community: result = std::make_shared<Community>(id); break;
        case proto::GeoGrid_Location_ContactCenter_Type_PrimaryCommunity:
                result = std::make_shared<PrimaryCommunity>(id);
                break;
        case proto::GeoGrid_Location_ContactCenter_Type_SecondaryCommunity:
                result = std::make_shared<SecondaryCommunity>(id);
                break;
        case proto::GeoGrid_Location_ContactCenter_Type_HighSchool: result = std::make_shared<HighSchool>(id); break;
        case proto::GeoGrid_Location_ContactCenter_Type_Household: result = std::make_shared<Household>(id); break;
        case proto::GeoGrid_Location_ContactCenter_Type_Workplace: result = std::make_shared<Workplace>(id); break;
        default: throw Exception("No such ContactCenter type");
        }

        auto e = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoContactCenter.pools_size(); idx++) {
                        std::shared_ptr<ContactPool>                             pool;
                        const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool =
                            protoContactCenter.pools(idx);
#pragma omp task firstprivate(protoContactPool, pool)
                        {
                                e->Run([&protoContactPool, &pool, this, &result] {
                                        pool = ParseContactPool(protoContactPool, result->getPoolSize());
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
} // namespace gengeopop

std::shared_ptr<ContactPool> GeoGridProtoReader::ParseContactPool(
    const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool, unsigned int poolSize)
{
        auto id     = static_cast<unsigned int>(protoContactPool.id());
        auto result = std::make_shared<ContactPool>(id, poolSize);

        for (int idx = 0; idx < protoContactPool.people_size(); idx++) {
                auto person_id = protoContactPool.people(idx);
                result->addMember(m_people.at(static_cast<const unsigned int&>(person_id)));
        }

        return result;
}

stride::Person* GeoGridProtoReader::ParsePerson(const std::shared_ptr<GeoGrid>& geoGrid,
                                                const proto::GeoGrid_Person&    protoPerson)
{
        auto id                   = protoPerson.id();
        auto age                  = protoPerson.age();
        auto schoolId             = protoPerson.school();
        auto householdId          = protoPerson.household();
        auto workplaceId          = protoPerson.workplace();
        auto primaryCommunityId   = protoPerson.primarycommunity();
        auto secondaryCommunityId = protoPerson.secondarycommunity();

        return geoGrid->CreatePerson(id, age, householdId, schoolId, workplaceId, primaryCommunityId,
                                     secondaryCommunityId);
}

} // namespace gengeopop
