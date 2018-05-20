#include "RegioSlicer.h"
#include <util/NestedIterator.h>

RegioSlicer& RegioSlicer::operator[](std::string regioName)
{
        // TODO make regionslicer refer to this region only
        return *this;
}

stride::util::NestedIterator<stride::Person*,
                             std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator,
                             std::vector<stride::Person*>::iterator>
RegioSlicer::begin()
{
        return stride::util::NestedIterator<
            stride::Person*, std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator,
            std::vector<stride::Person*>::iterator>(m_pools.begin(), m_pools.begin(), m_pools.end());
}

stride::util::NestedIterator<stride::Person*,
                             std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator,
                             std::vector<stride::Person*>::iterator>
RegioSlicer::end()
{
        return stride::util::NestedIterator<
            stride::Person*, std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator,
            std::vector<stride::Person*>::iterator>(m_pools.end(), m_pools.begin(), m_pools.end());
}

RegioSlicer& RegioSlicer::operator[](stride::ContactPoolType::Id id)
{
        // Slicer must be constrained to all contactPools of this type
        stride::ContactPoolSys t = m_pop->GetContactPoolSys();

        m_pools = t[id];
        std::cout << "Set pools: " << m_pools.size() << std::endl;

        return *this;
}

RegioSlicer::RegioSlicer(std::shared_ptr<stride::Population> pop) { m_pop = pop; }

std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator RegioSlicer::beginPools()
{
        return m_pools.begin();
}

std::vector<stride::ContactPool, std::allocator<stride::ContactPool>>::iterator RegioSlicer::endPools()
{
        return m_pools.begin();
}
