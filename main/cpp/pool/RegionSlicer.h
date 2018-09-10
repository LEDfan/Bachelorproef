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

#include "pool/ContactPool.h"
#include "pool/IdSubscriptArray.h"
#include "util/SegmentedVector.h"
#include "util/SliceIndexer.h"

#include <boost/range.hpp>

namespace stride {

/// A class that allows to index contactpools by region and contact pool type.
class RegionSlicer
{
public:
        /// The result when finally indexing
        using Result = boost::sub_range<util::SegmentedVector<ContactPool>>;

        /// The type for the ranges that can be indexed: first indexed by type, then by region id
        using Ranges =
            ContactPoolType::IdSubscriptArray<util::SliceIndexer<util::SegmentedVector<ContactPool>, std::size_t>>;

        /// Constructor, with region id and a Ranges (where to index)
        /// Make sure that `pool_sys_regions` is still alive as long as you want to index this object
        /// \see Population::SliceOnRegion
        RegionSlicer(std::size_t region_id, Ranges& pool_sys_regions);

        /// Index on contact pool type, the region id is given in the constructor
        /// Returns a range with all ContactPools of that type in that region
        Result operator[](ContactPoolType::Id type) const;

private:
        const std::size_t m_region_id;        ///< Keep track of the region until the final index
        Ranges&           m_pool_sys_regions; ///< What to index on, should be still alive
};

} // namespace stride
