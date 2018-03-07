#pragma once

#include "ContactCenter.h"

namespace gengeopop {
    class School : public ContactCenter {
    public:
        unsigned int getMaxPools();
        unsigned int getPoolSize();
    };
}
