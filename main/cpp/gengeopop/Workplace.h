#pragma once
#include "ContactCenter.h"

namespace gengeopop {

class Workplace : public ContactCenter
{
public:
        Workplace(unsigned int id);
        unsigned int getPoolSize() const override;
        unsigned int getMaxPools() const override;
        std::string  getType() const override;
        void         fill(GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
