#include "ContactPool.h"

namespace gengeopop {

ContactPool::ContactPool() : m_people(), m_capacity(0), m_id() {}

ContactPool::ContactPool(unsigned int id, unsigned long capacity) : m_people(), m_capacity(capacity), m_id(id) {}

void ContactPool::AddMember(std::shared_ptr<stride::Person> p) { m_people.emplace_back(std::move(p)); }

unsigned int ContactPool::GetID() const { return m_id; }

unsigned long ContactPool::GetCapacity() const { return m_capacity; }

unsigned long ContactPool::GetUsedCapacity() const { return m_people.size(); }

ContactPool::iterator gengeopop::ContactPool::begin() { return m_people.begin(); }

ContactPool::iterator gengeopop::ContactPool::end() { return m_people.end(); }

} // namespace gengeopop
