#ifndef STRIDE_SCHOOL_H
#define STRIDE_SCHOOL_H

#include "ContactCenter.h"

namespace gengeopop {
    class School : public ContactCenter {
    public:
        unsigned int getMaxPools();
        unsigned int getPoolSize();
    };
}



#endif //STRIDE_SCHOOL_H
