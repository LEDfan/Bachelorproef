#include "ContactCenter.h"

namespace gengeopop {

ContactCenter::ContactCenter(unsigned int id) : m_pools(), m_id(id) {}

void ContactCenter::addPool(std::shared_ptr<ContactPool> pool) { m_pools.emplace_back(std::move(pool)); }

ContactCenter::iterator ContactCenter::begin() { return m_pools.begin(); }

ContactCenter::iterator ContactCenter::end() { return m_pools.end(); }

const std::vector<std::shared_ptr<ContactPool>>& ContactCenter::GetPools() const { return m_pools; }

unsigned int ContactCenter::getId() const { return m_id; }

bool ContactCenter::isAvailable() const { return true; }

} // namespace gengeopop
