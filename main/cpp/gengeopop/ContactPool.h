#pragma once

#include "pop/Person.h"
#include <vector>

namespace gengeopop {
class ContactPool
{
public:
        ContactPool(unsigned int id, unsigned long capacity);
        ContactPool();

        using iterator       = std::vector<stride::Person*>::iterator;
        using const_iterator = std::vector<stride::Person*>::const_iterator;

        unsigned int  GetID() const;
        unsigned long GetCapacity() const;
        unsigned long GetUsedCapacity() const;
        void          AddMember(stride::Person* p);

        std::pair<unsigned int, unsigned int> GetPopulationAndInfectedCount() const;

        iterator begin();
        iterator end();

        const_iterator cbegin();
        const_iterator cend();

private:
        std::vector<stride::Person*> m_people;
        unsigned long                m_capacity;
        unsigned int                 m_id;
};
} // namespace gengeopop
