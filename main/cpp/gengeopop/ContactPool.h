#pragma once

#include <vector>
#include "pop/Person.h"

namespace gengeopop {

    class ContactPool {
    public:
        unsigned int getCapacity();
        unsigned int getUsedCapacity();
        void addMember(std::shared_ptr<stride::Person> p);

    private:
        std::vector<std::shared_ptr<stride::Person> > m_people;
        unsigned int m_capacity = 0;
    };
}
