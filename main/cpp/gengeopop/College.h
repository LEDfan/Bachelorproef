#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class College : public ContactCenter
{
public:
        explicit College(unsigned int id);
        std::string  GetType() const override;
        unsigned int GetPoolSize() const override;
        unsigned int GetMaxPools() const override;
        void         Fill(GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
