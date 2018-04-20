#pragma once
#include <set>

#include "GeoGridReader.h"
#include "proto/geogrid.pb.h"

namespace gengeopop {

class GeoGridProtoReader : public GeoGridReader
{
public:
        GeoGridProtoReader(std::unique_ptr<std::istream> inputStream);

        std::shared_ptr<GeoGrid> read() override;

private:
        std::shared_ptr<Location>      ParseLocation(const proto::GeoGrid_Location& protoLocation);
        Coordinate                     ParseCoordinate(const proto::GeoGrid_Location_Coordinate& protoCoordinate);
        std::shared_ptr<ContactCenter> ParseContactCenter(
            const proto::GeoGrid_Location_ContactCenter& protoContactCenter);
        std::shared_ptr<ContactPool> ParseContactPool(
            const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool, unsigned int poolSize);
        stride::Person* ParsePerson(const std::shared_ptr<GeoGrid>& protoPerson, const proto::GeoGrid_Person& person);
};
} // namespace gengeopop
