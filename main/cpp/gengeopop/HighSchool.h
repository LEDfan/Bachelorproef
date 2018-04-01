#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class HighSchool : public ContactCenter
{
public:
        HighSchool(unsigned int id);
        std::string  getType() const;
        unsigned int getPoolSize() const;
        unsigned int getMaxPools() const;
        void         fill(GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
