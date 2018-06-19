#pragma once

#include "pool/ContactPool.h"
#include "pool/IdSubscriptArray.h"
#include "util/RangeIndexer.h"
#include "util/SegmentedVector.h"

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
            ContactPoolType::IdSubscriptArray<util::RangeIndexer<util::SegmentedVector<ContactPool>, std::size_t>>;

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
