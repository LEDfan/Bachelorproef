#include "GeoGridProtoWriter.h"
#include "proto/geogrid.pb.h"
#include <iostream>
#include <omp.h>

namespace gengeopop {

GeoGridProtoWriter::GeoGridProtoWriter() : m_persons_found() {}

void GeoGridProtoWriter::write(std::shared_ptr<gengeopop::GeoGrid> geoGrid, std::ostream& stream)
{
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        proto::GeoGrid protoGrid;
        for (const auto& location : *geoGrid) {
                writeLocation(location, protoGrid.add_locations());
        }
        for (const auto& person : m_persons_found) {
                writePerson(person, protoGrid.add_persons());
        }

        m_persons_found.clear();
        if (!protoGrid.SerializeToOstream(&stream)) {
                throw std::runtime_error("There was an error writing the GeoGrid to the file.");
        }
        google::protobuf::ShutdownProtobufLibrary();
        stream.flush();
}

void GeoGridProtoWriter::writeLocation(std::shared_ptr<Location> location, proto::GeoGrid_Location* protoLocation)
{
        protoLocation->set_id(location->getID());
        protoLocation->set_name(location->getName());
        protoLocation->set_province(location->getProvince());
        protoLocation->set_population(location->getPopulation());
        auto coordinate = new proto::GeoGrid_Location_Coordinate();
        writeCoordinate(location->getCoordinate(), coordinate);
        protoLocation->set_allocated_coordinate(coordinate);

        auto commutes = location->getOutgoingCommuningCities();
        for (auto commute_pair : commutes) {
                auto commute = protoLocation->add_commutes();
                commute->set_to(commute_pair.first->getID());
                commute->set_proportion(commute_pair.second);
        }

        for (const auto& contactCenter : *location) {
                writeContactCenter(contactCenter, protoLocation->add_contactcenters());
        }
        for (const auto& subMunicipality : location->getSubMunicipalities()) {
                protoLocation->add_submunicipalities(subMunicipality->getID());
        }
}

void GeoGridProtoWriter::writeCoordinate(const Coordinate&                   coordinate,
                                         proto::GeoGrid_Location_Coordinate* protoCoordinate)
{
        protoCoordinate->set_x(coordinate.x);
        protoCoordinate->set_y(coordinate.y);
        protoCoordinate->set_longitude(coordinate.longitude);
        protoCoordinate->set_latitude(coordinate.latitude);
}

void GeoGridProtoWriter::writeContactCenter(std::shared_ptr<ContactCenter>         contactCenter,
                                            proto::GeoGrid_Location_ContactCenter* protoContactCenter)
{
        protoContactCenter->set_id(contactCenter->getId());
        protoContactCenter->set_type(contactCenter->getType());
        for (const auto& pool : *contactCenter) {
                writeContactPool(pool, protoContactCenter->add_pools());
        }
}

void GeoGridProtoWriter::writeContactPool(std::shared_ptr<ContactPool>                       contactPool,
                                          proto::GeoGrid_Location_ContactCenter_ContactPool* protoContactPool)
{
        protoContactPool->set_id(contactPool->getID());
        for (const auto& person : *contactPool) {
                protoContactPool->add_people(person->GetId());
                m_persons_found.insert(person);
        }
}
void GeoGridProtoWriter::writePerson(std::shared_ptr<stride::Person> person, proto::GeoGrid_Person* protoPerson)
{
        protoPerson->set_id(person->GetId());
        protoPerson->set_age(person->GetAge());
        protoPerson->set_gender(std::string(1, person->GetGender()));
        protoPerson->set_school(person->GetSchoolId());
        protoPerson->set_household(person->GetHouseholdId());
        protoPerson->set_workplace(person->GetWorkId());
        protoPerson->set_primarycommunity(person->GetPrimaryCommunityId());
        protoPerson->set_secondarycommunity(person->GetSecondaryCommunityId());
}
} // namespace gengeopop
