#pragma once
#include "ContactCenter.h"

namespace gengeopop {

    class Workplace: public ContactCenter {
    public:
        unsigned int getPoolSize() const;
        unsigned int getMaxPools() const;
        std::string getType() const;
    };
}
