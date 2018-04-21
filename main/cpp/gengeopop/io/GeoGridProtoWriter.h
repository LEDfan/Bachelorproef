#pragma once
#include <set>

#include "GeoGridWriter.h"
#include "proto/geogrid.pb.h"

namespace gengeopop {

class GeoGridProtoWriter : public GeoGridWriter
{
public:
        GeoGridProtoWriter();

        void write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream) override;

private:
        void writeLocation(std::shared_ptr<Location> location, proto::GeoGrid_Location* protoLocation);
        void writeCoordinate(const Coordinate& coordinate, proto::GeoGrid_Location_Coordinate* protoCoordinate);
        void writeContactCenter(std::shared_ptr<ContactCenter>         contactCenter,
                                proto::GeoGrid_Location_ContactCenter* protoContactCenter);
        void writeContactPool(std::shared_ptr<ContactPool>                       contactPool,
                              proto::GeoGrid_Location_ContactCenter_ContactPool* protoContactPool);
        void writePerson(stride::Person* person, proto::GeoGrid_Person* protoPerson);

        std::set<stride::Person*> m_persons_found;
};
} // namespace gengeopop
