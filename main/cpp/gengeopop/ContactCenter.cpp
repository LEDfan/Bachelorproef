#include "ContactCenter.h"

namespace gengeopop {

ContactCenter::ContactCenter(unsigned int id) : m_pools(), m_id(id) {}

void ContactCenter::AddPool(stride::ContactPool* pool) { m_pools.emplace_back(pool); }

ContactCenter::iterator ContactCenter::begin() { return m_pools.begin(); }

ContactCenter::iterator ContactCenter::end() { return m_pools.end(); }

const std::vector<stride::ContactPool*>& ContactCenter::GetPools() const { return m_pools; }

unsigned int ContactCenter::GetId() const { return m_id; }

std::pair<unsigned int, unsigned int> ContactCenter::GetPopulationAndInfectedCount() const
{
        unsigned int population = 0;
        unsigned int infected   = 0;

        for (stride::ContactPool* pool : m_pools) {
                auto poolR = pool->GetPopulationAndInfectedCount();
                population += poolR.first;
                infected += poolR.second;
        }

        return {population, infected};
}

} // namespace gengeopop
