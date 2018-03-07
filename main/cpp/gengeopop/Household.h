#ifndef STRIDE_HOUSEHOLD_H
#define STRIDE_HOUSEHOLD_H


#include "ContactCenter.h"

namespace gengeopop {

    class Household : public ContactCenter {
    public:
        unsigned int getMaxPools();
        unsigned int getPoolSize();

    };

}


#endif //STRIDE_HOUSEHOLD_H
