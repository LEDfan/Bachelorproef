#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class School : public ContactCenter
{
public:
        explicit School(unsigned int id);
        std::string  getType() const override;
        unsigned int getMaxPools() const override;
        unsigned int getPoolSize() const override;
        void         fill(GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
