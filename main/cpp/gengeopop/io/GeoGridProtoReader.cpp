#include "GeoGridProtoReader.h"
#include "ThreadException.h"
#include "proto/geogrid.pb.h"
#include <gengeopop/College.h>
#include <gengeopop/Community.h>
#include <gengeopop/Household.h>
#include <gengeopop/K12School.h>
#include <gengeopop/PrimaryCommunity.h>
#include <gengeopop/SecondaryCommunity.h>
#include <gengeopop/Workplace.h>
#include <iostream>
#include <omp.h>
#include <stdexcept>
#include <util/Exception.h>

namespace gengeopop {

GeoGridProtoReader::GeoGridProtoReader(std::unique_ptr<std::istream> inputStream)
    : GeoGridReader(std::move(inputStream)), m_geoGrid()
{
}

std::shared_ptr<GeoGrid> GeoGridProtoReader::Read()
{
        proto::GeoGrid protoGrid;
        if (!protoGrid.ParseFromIstream(m_inputStream.get())) {
                throw stride::util::Exception("Failed to parse Proto file");
        }
        if (m_population) {
                m_geoGrid = std::make_shared<GeoGrid>(m_population, m_regionId);
        } else {
                m_geoGrid = std::make_shared<GeoGrid>();
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
                                        person                    = ParsePerson(protoPerson);
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
                                        m_geoGrid->AddLocation(std::move(loc));
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();
        AddCommutes(m_geoGrid);
        AddSubMunicipalities(m_geoGrid);
        m_people.clear();
        m_commutes.clear();
        m_subMunicipalities.clear();
        return m_geoGrid;
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
                                        result->AddContactCenter(center);
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
#pragma omp critical
                m_subMunicipalities.emplace_back(std::make_pair(result->GetID(), protoLocation.submunicipalities(idx)));
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
        stride::ContactPoolType::Id                typeId;

        switch (type) {
        case proto::GeoGrid_Location_ContactCenter_Type_K12School:
                result = std::make_shared<K12School>(id);
                typeId = stride::ContactPoolType::Id::K12School;
                break;
        case proto::GeoGrid_Location_ContactCenter_Type_PrimaryCommunity:
                result = std::make_shared<PrimaryCommunity>(id);
                typeId = stride::ContactPoolType::Id::PrimaryCommunity;
                break;
        case proto::GeoGrid_Location_ContactCenter_Type_SecondaryCommunity:
                result = std::make_shared<SecondaryCommunity>(id);
                typeId = stride::ContactPoolType::Id::SecondaryCommunity;
                break;
        case proto::GeoGrid_Location_ContactCenter_Type_College:
                result = std::make_shared<College>(id);
                typeId = stride::ContactPoolType::Id::College;
                break;
        case proto::GeoGrid_Location_ContactCenter_Type_Household:
                result = std::make_shared<Household>(id);
                typeId = stride::ContactPoolType::Id::Household;
                break;
        case proto::GeoGrid_Location_ContactCenter_Type_Workplace:
                result = std::make_shared<Workplace>(id);
                typeId = stride::ContactPoolType::Id::Work;
                break;
                break;
        default: throw stride::util::Exception("No such ContactCenter type");
        }

        auto e = std::make_shared<ThreadException>();
#pragma omp parallel
#pragma omp single
        {
                for (int idx = 0; idx < protoContactCenter.pools_size(); idx++) {
                        stride::ContactPool*                                     pool;
                        const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool =
                            protoContactCenter.pools(idx);
#pragma omp task firstprivate(protoContactPool, pool, typeId)
                        {
                                e->Run([&protoContactPool, &pool, this, &typeId] {
                                        pool = ParseContactPool(protoContactPool, typeId);
                                });
                                if (!e->HasError())
#pragma omp critical
                                        result->AddPool(pool);
                        }
                }
#pragma omp taskwait
        }
        e->Rethrow();
        return result;
}

stride::ContactPool* GeoGridProtoReader::ParseContactPool(
    const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool, stride::ContactPoolType::Id type)
{
        // Don't use the id of the ContactPool but the let the Population create an id
        stride::ContactPool* result;

#pragma omp critical
        result = m_geoGrid->CreateContactPool(type);

        for (int idx = 0; idx < protoContactPool.people_size(); idx++) {
                auto person_id = static_cast<unsigned int>(protoContactPool.people(idx));
                auto person    = m_people.at(person_id);

#pragma omp critical
                {
                        result->AddMember(person);
                        // Update original pool id with new pool id used in the population
                        person->SetPoolId(type, result->GetId());
                }
        }

        return result;
}

stride::Person* GeoGridProtoReader::ParsePerson(const proto::GeoGrid_Person& protoPerson)
{
        auto id  = protoPerson.id();
        auto age = protoPerson.age();

        return m_geoGrid->CreatePerson(id, age, 0, 0, 0, 0, 0, 0);
}

} // namespace gengeopop
