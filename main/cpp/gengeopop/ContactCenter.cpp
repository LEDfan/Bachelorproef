#include "ContactCenter.h"

namespace gengeopop {

ContactCenter::ContactCenter(unsigned int id) : m_pools(), m_id(id) {}

void ContactCenter::addPool(std::shared_ptr<gengeopop::ContactPool> pool) { m_pools.push_back(pool); }

ContactCenter::iterator ContactCenter::begin() { return m_pools.begin(); }

ContactCenter::iterator ContactCenter::end() { return m_pools.end(); }

const std::vector<std::shared_ptr<ContactPool>>& ContactCenter::GetPools() const { return m_pools; }

unsigned int ContactCenter::getId() const { return m_id; }

} // namespace gengeopop
