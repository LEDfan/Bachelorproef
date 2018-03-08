#include "ContactPool.h"

void gengeopop::ContactPool::addMember(std::shared_ptr<stride::Person> p) {
    m_people.push_back(p);
}
