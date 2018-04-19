#pragma once
#include <set>

#include "GeoGridReader.h"
#include "proto/geogrid.pb.h"

namespace gengeopop {

class GeoGridProtoReader : public GeoGridReader
{
public:
        GeoGridProtoReader();

        std::shared_ptr<GeoGrid> read(std::istream& stream) override;

private:
        std::shared_ptr<Location>      ParseLocation(const proto::GeoGrid_Location& protoLocation);
        Coordinate                     ParseCoordinate(const proto::GeoGrid_Location_Coordinate& protoCoordinate);
        std::shared_ptr<ContactCenter> ParseContactCenter(
            const proto::GeoGrid_Location_ContactCenter& protoContactCenter);
        std::shared_ptr<ContactPool> ParseContactPool(
            const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool, unsigned int poolSize);
        std::shared_ptr<stride::Person> ParsePerson(const proto::GeoGrid_Person& protoPerson);
};
} // namespace gengeopop
