#pragma once

#include <vector>
#include "pop/Person.h"

namespace gengeopop {
    class ContactPool {
    public:
        using iterator = std::vector<std::shared_ptr<stride::Person>>::iterator;
        unsigned int getID() const;
        unsigned long getCapacity() const;
        unsigned long getUsedCapacity() const;
        void addMember(std::shared_ptr<stride::Person> p);

        iterator begin();
        iterator end();

    private:
        std::vector<std::shared_ptr<stride::Person> > m_people;
        unsigned long m_capacity = 0;
        unsigned int m_id;
    };
}
