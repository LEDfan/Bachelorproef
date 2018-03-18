#include "ContactPool.h"

namespace gengeopop {

void ContactPool::addMember(std::shared_ptr<stride::Person> p) { m_people.push_back(p); }

unsigned int ContactPool::getID() const { return m_id; }

unsigned long ContactPool::getCapacity() const { return m_capacity; }

unsigned long ContactPool::getUsedCapacity() const { return m_people.size(); }

ContactPool::iterator gengeopop::ContactPool::begin() { return m_people.begin(); }

ContactPool::iterator gengeopop::ContactPool::end() { return m_people.end(); }

ContactPool::ContactPool(unsigned int id): m_id(id) {}

} // namespace gengeopop
