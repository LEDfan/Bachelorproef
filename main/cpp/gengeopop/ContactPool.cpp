#include "ContactPool.h"

void gengeopop::ContactPool::addMember(std::shared_ptr<stride::Person> p) {
    m_people.push_back(p);
}

unsigned int gengeopop::ContactPool::getID() {
    return m_id;
}

unsigned long gengeopop::ContactPool::getCapacity() {
    return m_capacity;
}

unsigned long gengeopop::ContactPool::getUsedCapacity() {
    return m_people.size();
}

gengeopop::ContactPoolIterator gengeopop::ContactPool::begin() {
    return m_people.begin();
}

gengeopop::ContactPoolIterator gengeopop::ContactPool::end() {
    return m_people.end();
}
