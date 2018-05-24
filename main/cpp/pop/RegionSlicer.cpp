#include "RegionSlicer.h"
#include "Population.h"
#include <util/NestedIterator.h>

namespace stride {

RegionSlicer& RegionSlicer::operator[](const std::string& regioName) { return *this; }

RegionSlicer::person_iterator RegionSlicer::begin()
{
        return RegionSlicer::person_iterator(m_pools.begin(), m_pools.begin(), m_pools.end());
}

RegionSlicer::person_iterator RegionSlicer::end()
{
        return RegionSlicer::person_iterator(m_pools.end(), m_pools.begin(), m_pools.end());
}

RegionSlicer& RegionSlicer::operator[](stride::ContactPoolType::Id id) { return *this; }

RegionSlicer::RegionSlicer(stride::Population* pop) : m_pop(pop) {}

RegionSlicer::contactpool_iterator RegionSlicer::beginPools() { return m_pools.begin(); }

RegionSlicer::contactpool_iterator RegionSlicer::endPools() { return m_pools.begin(); }

void RegionSlicer::UpdatePools()
{
        // TODO
}
} // namespace stride
