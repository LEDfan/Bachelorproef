#pragma once

#include "ContactCenter.h"

namespace gengeopop {

class Household : public ContactCenter
{
public:
        Household(unsigned int id);
        Household();
        std::string  getType() const override;
        unsigned int getMaxPools() const override;
        unsigned int getPoolSize() const override;
        void         fill(GeoGridConfig& geoGridConfig) override;
        virtual ~Household(){};
};

} // namespace gengeopop
