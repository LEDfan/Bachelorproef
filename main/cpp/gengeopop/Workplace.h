#pragma once
#include "ContactCenter.h"

namespace gengeopop {

    class Workplace: public ContactCenter {
    public:
        unsigned int getPoolSize();
        std::string getType();
    };
}
