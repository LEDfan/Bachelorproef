#pragma once
#include <set>

#include "GeoGridReader.h"
#include "proto/geogrid.pb.h"

namespace gengeopop {

/**
 * An implementation of the GeoGridReader using Protocol Buffers
 * This class is used to read a GeoGrid from a Proto file
 */

class GeoGridProtoReader : public GeoGridReader
{
public:
        /// Construct the GeoGridJSONReader with the istream which contains the Protobuf information
        GeoGridProtoReader(std::unique_ptr<std::istream> inputStream);

        /// Actually perform the read and return the GeoGrid
        std::shared_ptr<GeoGrid> Read() override;

private:
        /// Create a Location based on the provided protobuf Location
        std::shared_ptr<Location> ParseLocation(const proto::GeoGrid_Location& protoLocation);

        /// Create a Coordinate based on the provided protobuf Coordinate
        Coordinate ParseCoordinate(const proto::GeoGrid_Location_Coordinate& protoCoordinate);

        /// Create a ContactCenter based on the provided protobuf ContactCenter
        std::shared_ptr<ContactCenter> ParseContactCenter(
            const proto::GeoGrid_Location_ContactCenter& protoContactCenter);

        /// Create a ContactPool based on the provided protobuf ContactPool
        stride::ContactPool* ParseContactPool(const proto::GeoGrid_Location_ContactCenter_ContactPool& protoContactPool,
                                              stride::ContactPoolType::Id                              typeId);

        /// Create a Person based on the provided protobuf Person
        stride::Person* ParsePerson(const proto::GeoGrid_Person& person);

        std::shared_ptr<GeoGrid> m_geoGrid; ///< The GeoGrid which is being built
};
} // namespace gengeopop
