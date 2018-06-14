#pragma once

#include "pool/ContactPool.h"
#include "pool/IdSubscriptArray.h"
#include "util/RangeIndexer.h"
#include "util/SegmentedVector.h"

#include <boost/range.hpp>

namespace stride {

class RegionSlicer
{
public:
        using Result = boost::sub_range<util::SegmentedVector<ContactPool>>;
        using Ranges =
            ContactPoolType::IdSubscriptArray<util::RangeIndexer<util::SegmentedVector<ContactPool>, std::size_t>>;

        RegionSlicer(std::size_t region_id, const Ranges& pool_sys_regions);

        Result operator[](ContactPoolType::Id type) const;

private:
        const std::size_t m_region_id;
        const Ranges&     m_pool_sys_regions;
};

} // namespace stride
