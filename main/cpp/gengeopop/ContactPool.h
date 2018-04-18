#pragma once

#include "pop/Person.h"
#include <vector>

namespace gengeopop {
class ContactPool
{
public:
        ContactPool(unsigned int id, unsigned long capacity);
        ContactPool();

        using iterator = std::vector<std::shared_ptr<stride::Person>>::iterator;

        unsigned int  GetID() const;
        unsigned long GetCapacity() const;
        unsigned long GetUsedCapacity() const;
        void          AddMember(std::shared_ptr<stride::Person> p);

        iterator begin();
        iterator end();

private:
        std::vector<std::shared_ptr<stride::Person>> m_people;
        unsigned long                                m_capacity;
        unsigned int                                 m_id;
};
} // namespace gengeopop
