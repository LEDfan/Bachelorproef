#include "ContactPool.h"

namespace gengeopop {

ContactPool::ContactPool() : m_people(), m_capacity(0), m_id() {}

void ContactPool::addMember(std::shared_ptr<stride::Person> p) { m_people.push_back(p); }

unsigned int ContactPool::getID() const { return m_id; }

unsigned long ContactPool::getCapacity() const { return m_capacity; }

unsigned long ContactPool::getUsedCapacity() const { return m_people.size(); }

ContactPool::iterator gengeopop::ContactPool::begin() { return m_people.begin(); }

ContactPool::iterator gengeopop::ContactPool::end() { return m_people.end(); }

} // namespace gengeopop
