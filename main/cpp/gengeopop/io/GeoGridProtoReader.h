#pragma once
#include <set>

#include "GeoGridReader.h"
#include "proto/geogrid.pb.h"

namespace gengeopop {

class GeoGridProtoReader : public GeoGridReader
{
public:
        GeoGridProtoReader(std::unique_ptr<std::istream> inputStream);

        std::shared_ptr<GeoGrid> Read() override;

private:
        std::shared_ptr<Location>      ParseLocation(const proto::GeoGrid_Location& protoLocation);
        Coordinate                     ParseCoordinate(const proto::GeoGrid_Location_Coordinate& protoCoordinate);
        std::shared_ptr<ContactCenter> ParseContactCenter(
            const proto::GeoGrid_Location_ContactCenter& protoContactCenter);
        stride::ContactPool* ParseContactPool(const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool,
                                              stride::ContactPoolType::Id                              typeId);
        stride::Person*      ParsePerson(const proto::GeoGrid_Person& person);
        std::shared_ptr<GeoGrid> m_geoGrid;
};
} // namespace gengeopop
