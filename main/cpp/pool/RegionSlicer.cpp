#include "RegionSlicer.h"

namespace stride {

RegionSlicer::RegionSlicer(std::size_t region_id, RegionSlicer::Ranges& pool_sys_regions)
    : m_region_id(region_id), m_pool_sys_regions(pool_sys_regions)
{
}

RegionSlicer::Result RegionSlicer::operator[](ContactPoolType::Id type) const
{
        return m_pool_sys_regions[type].GetRange(m_region_id);
}

} // namespace stride
