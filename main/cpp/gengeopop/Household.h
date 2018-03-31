#pragma once

#include "ContactCenter.h"

namespace gengeopop {

class Household : public ContactCenter
{
public:
        Household(unsigned int id);
        Household();
        std::string  getType() const;
        unsigned int getMaxPools() const;
        unsigned int getPoolSize() const;
        virtual ~Household(){};
};

} // namespace gengeopop
