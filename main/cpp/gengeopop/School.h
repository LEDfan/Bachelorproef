#pragma once

#include "ContactCenter.h"

namespace gengeopop {
    class School : public ContactCenter {
    public:
        std::string getType();
        unsigned int getMaxPools() const;
        unsigned int getPoolSize() const;
    };
}
