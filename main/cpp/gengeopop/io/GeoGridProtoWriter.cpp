#include "GeoGridProtoWriter.h"
#include "proto/geogrid.pb.h"
#include <iostream>
#include <omp.h>
#include <util/Exception.h>

namespace gengeopop {

GeoGridProtoWriter::GeoGridProtoWriter() : m_persons_found() {}

void GeoGridProtoWriter::Write(std::shared_ptr<gengeopop::GeoGrid> geoGrid, std::ostream& stream)
{
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        proto::GeoGrid protoGrid;
        for (const auto& location : *geoGrid) {
                WriteLocation(location, protoGrid.add_locations());
        }
        for (const auto& person : m_persons_found) {
                WritePerson(person, protoGrid.add_persons());
        }

        m_persons_found.clear();
        if (!protoGrid.SerializeToOstream(&stream)) {
                throw stride::util::Exception("There was an error writing the GeoGrid to the file.");
        }
        google::protobuf::ShutdownProtobufLibrary();
        stream.flush();
}

void GeoGridProtoWriter::WriteLocation(std::shared_ptr<Location> location, proto::GeoGrid_Location* protoLocation)
{
        protoLocation->set_id(location->GetID());
        protoLocation->set_name(location->GetName());
        protoLocation->set_province(location->GetProvince());
        protoLocation->set_population(location->GetPopulation());
        auto coordinate = new proto::GeoGrid_Location_Coordinate();
        WriteCoordinate(location->GetCoordinate(), coordinate);
        protoLocation->set_allocated_coordinate(coordinate);

        auto commutes = location->GetOutgoingCommuningCities();
        for (auto commute_pair : commutes) {
                auto commute = protoLocation->add_commutes();
                commute->set_to(commute_pair.first->GetID());
                commute->set_proportion(commute_pair.second);
        }

        for (const auto& contactCenter : *location) {
                WriteContactCenter(contactCenter, protoLocation->add_contactcenters());
        }
        for (const auto& subMunicipality : location->GetSubMunicipalities()) {
                protoLocation->add_submunicipalities(subMunicipality->GetID());
        }
}

void GeoGridProtoWriter::WriteCoordinate(const Coordinate&                   coordinate,
                                         proto::GeoGrid_Location_Coordinate* protoCoordinate)
{
        protoCoordinate->set_x(coordinate.x);
        protoCoordinate->set_y(coordinate.y);
        protoCoordinate->set_longitude(coordinate.longitude);
        protoCoordinate->set_latitude(coordinate.latitude);
}

void GeoGridProtoWriter::WriteContactCenter(std::shared_ptr<ContactCenter>         contactCenter,
                                            proto::GeoGrid_Location_ContactCenter* protoContactCenter)
{
        std::map<std::string, proto::GeoGrid_Location_ContactCenter_Type> types = {
            {"School", proto::GeoGrid_Location_ContactCenter_Type_School},
            {"Community", proto::GeoGrid_Location_ContactCenter_Type_Community},
            {"PrimaryCommunity", proto::GeoGrid_Location_ContactCenter_Type_PrimaryCommunity},
            {"SecondaryCommunity", proto::GeoGrid_Location_ContactCenter_Type_SecondaryCommunity},
            {"College", proto::GeoGrid_Location_ContactCenter_Type_College},
            {"Household", proto::GeoGrid_Location_ContactCenter_Type_Household},
            {"Workplace", proto::GeoGrid_Location_ContactCenter_Type_Workplace}};

        protoContactCenter->set_id(contactCenter->GetId());
        protoContactCenter->set_type(types[contactCenter->GetType()]);
        for (const auto& pool : *contactCenter) {
                WriteContactPool(pool, protoContactCenter->add_pools());
        }
}

void GeoGridProtoWriter::WriteContactPool(std::shared_ptr<ContactPool>                       contactPool,
                                          proto::GeoGrid_Location_ContactCenter_ContactPool* protoContactPool)
{
        protoContactPool->set_id(contactPool->GetID());
        for (const auto& person : *contactPool) {
                protoContactPool->add_people(person->GetId());
                m_persons_found.insert(person);
        }
}

void GeoGridProtoWriter::WritePerson(stride::Person* person, proto::GeoGrid_Person* protoPerson)
{
        protoPerson->set_id(person->GetId());
        protoPerson->set_age(static_cast<google::protobuf::int64>(person->GetAge()));
        protoPerson->set_gender(std::string(1, person->GetGender()));
        protoPerson->set_school(person->GetSchoolId());
        protoPerson->set_household(person->GetHouseholdId());
        protoPerson->set_workplace(person->GetWorkId());
        protoPerson->set_primarycommunity(person->GetPrimaryCommunityId());
        protoPerson->set_secondarycommunity(person->GetSecondaryCommunityId());
}
} // namespace gengeopop
