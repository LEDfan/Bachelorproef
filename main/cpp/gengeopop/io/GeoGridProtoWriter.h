#pragma once
#include <set>

#include "GeoGridWriter.h"
#include "proto/geogrid.pb.h"

namespace gengeopop {

class GeoGridProtoWriter : public GeoGridWriter
{
public:
        GeoGridProtoWriter();

        void Write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream) override;

private:
        void WriteLocation(std::shared_ptr<Location> location, proto::GeoGrid_Location* protoLocation);
        void WriteCoordinate(const Coordinate& coordinate, proto::GeoGrid_Location_Coordinate* protoCoordinate);
        void WriteContactCenter(std::shared_ptr<ContactCenter>         contactCenter,
                                proto::GeoGrid_Location_ContactCenter* protoContactCenter);
        void WriteContactPool(std::shared_ptr<ContactPool>                       contactPool,
                              proto::GeoGrid_Location_ContactCenter_ContactPool* protoContactPool);
        void WritePerson(std::shared_ptr<stride::Person> person, proto::GeoGrid_Person* protoPerson);

        std::set<std::shared_ptr<stride::Person>> m_persons_found;
};
} // namespace gengeopop
