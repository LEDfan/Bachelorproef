#include "ContactCenter.h"

void gengeopop::ContactCenter::addPool(std::shared_ptr<gengeopop::ContactPool> pool) {
    m_pools.push_back(pool);
}

gengeopop::ContactCenterIterator gengeopop::ContactCenter::begin() {
    return m_pools.begin();
}

gengeopop::ContactCenterIterator gengeopop::ContactCenter::end() {
    return m_pools.end();
}
