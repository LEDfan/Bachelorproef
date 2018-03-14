#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class HighSchool : public ContactCenter
{
        std::string  getType() const;
        unsigned int getPoolSize() const;
        unsigned int getMaxPools() const;
};
} // namespace gengeopop
