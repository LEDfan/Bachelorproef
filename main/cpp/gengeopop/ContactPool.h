#ifndef STRIDE_GENGEOPOP_CONTACTPOOL_H
#define STRIDE_GENGEOPOP_CONTACTPOOL_H

#include "pop/Person.h"

namespace gengeopop {

    class ContactPool {
    public:
        unsigned int getCapacity();
        unsigned int getUsedCapacity();
        void addMember(std::shared_ptr<stride::Person> p);


    private:
        std::vector<stride::Person> m_people;
        unsigned int m_capacity = 0;
    };
}

#endif //STRIDE_CONTACTPOOL_H
