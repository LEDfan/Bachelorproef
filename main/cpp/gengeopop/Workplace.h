#pragma once
#include "ContactCenter.h"

namespace gengeopop {

class Workplace : public ContactCenter
{
public:
        Workplace(unsigned int id);
        unsigned int getPoolSize() const;
        unsigned int getMaxPools() const;
        std::string  getType() const;
};
} // namespace gengeopop
