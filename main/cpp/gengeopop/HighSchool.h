#pragma once

#include "ContactCenter.h"

namespace gengeopop {
    class HighSchool : public ContactCenter {
            std::string getType();
            unsigned int getPoolSize() const;
            unsigned int getMaxPools() const;

    };
}

