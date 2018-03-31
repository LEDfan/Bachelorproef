#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class School : public ContactCenter
{
public:
        School(unsigned int id);
        std::string  getType() const;
        unsigned int getMaxPools() const;
        unsigned int getPoolSize() const;
};
} // namespace gengeopop
