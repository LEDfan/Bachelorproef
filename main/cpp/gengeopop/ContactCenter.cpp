#include "ContactCenter.h"

namespace gengeopop {

ContactCenter::ContactCenter() : m_pools() {}

void ContactCenter::addPool(std::shared_ptr<gengeopop::ContactPool> pool) { m_pools.push_back(pool); }

ContactCenter::iterator ContactCenter::begin() { return m_pools.begin(); }

ContactCenter::iterator ContactCenter::end() { return m_pools.end(); }

} // namespace gengeopop
