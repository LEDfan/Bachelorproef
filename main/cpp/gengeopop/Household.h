#pragma once

#include "ContactCenter.h"

namespace gengeopop {

    class Household : public ContactCenter {
    public:
        unsigned int getMaxPools();
        unsigned int getPoolSize();
        std::string getType();
    };

}
