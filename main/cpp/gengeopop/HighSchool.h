#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class HighSchool : public ContactCenter
{
public:
        explicit HighSchool(unsigned int id);
        std::string  getType() const override;
        unsigned int getPoolSize() const override;
        unsigned int getMaxPools() const override;
        void         fill(GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
