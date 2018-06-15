#pragma once
#include <set>

#include "GeoGridWriter.h"
#include "proto/geogrid.pb.h"

namespace gengeopop {

/**
 * An implementation of the GeoGridWriter using Protocol Buffers
 * This class is used to write a GeoGrid to a Proto file
 */
class GeoGridProtoWriter : public GeoGridWriter
{
public:
        /// Construct the GeoGridProtoWriter
        GeoGridProtoWriter();

        /// Write the provided GeoGrid to the proved ostream in Protobuf format
        void Write(std::shared_ptr<GeoGrid> geoGrid, std::ostream& stream) override;

private:
        /// Create a ProtoBuf Location containing all the information needed to reconstruct a Location
        void WriteLocation(std::shared_ptr<Location> location, proto::GeoGrid_Location* protoLocation);

        /// Create a ProtoBuf Coordinate containing all the information needed to reconstruct a Coordinate
        void WriteCoordinate(const Coordinate& coordinate, proto::GeoGrid_Location_Coordinate* protoCoordinate);

        /// Create a ProtoBuf ContactCenter containing all the information needed to reconstruct a ContactCenter
        void WriteContactCenter(std::shared_ptr<ContactCenter>         contactCenter,
                                proto::GeoGrid_Location_ContactCenter* protoContactCenter);

        /// Create a ProtoBuf ContactPool containing all the information needed to reconstruct a ContactPool
        void WriteContactPool(stride::ContactPool*                               contactPool,
                              proto::GeoGrid_Location_ContactCenter_ContactPool* protoContactPool);

        /// Create a ProtoBuf Person containing all the information needed to reconstruct a Person
        void WritePerson(stride::Person* person, proto::GeoGrid_Person* protoPerson);

        std::set<stride::Person*>
            m_persons_found; ///< The persons found when looping over the ContactPools.
                             ///< These are needed since they are stored in a separate list inside the JSON file.
};
} // namespace gengeopop
