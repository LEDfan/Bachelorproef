#pragma once

#include <vector>
#include "pop/Person.h"

namespace gengeopop {
    using ContactPoolIterator = std::vector<std::shared_ptr<stride::Person>>::iterator;
    class ContactPool {
    public:
        unsigned int getID();
        unsigned long getCapacity();
        unsigned long getUsedCapacity();
        void addMember(std::shared_ptr<stride::Person> p);

        ContactPoolIterator begin();
        ContactPoolIterator end();

    private:
        std::vector<std::shared_ptr<stride::Person> > m_people;
        unsigned long m_capacity = 0;
        unsigned int m_id;
    };
}
