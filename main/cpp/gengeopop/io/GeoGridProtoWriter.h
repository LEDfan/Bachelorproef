/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Niels Aerens, Thomas Avé, Tobia De Koninck, Robin Jadoul
 */

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
