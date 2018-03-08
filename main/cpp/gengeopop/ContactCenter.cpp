#include "ContactCenter.h"

void gengeopop::ContactCenter::addPool(std::shared_ptr<gengeopop::ContactPool> pool) {
    m_pools.push_back(pool);
}
