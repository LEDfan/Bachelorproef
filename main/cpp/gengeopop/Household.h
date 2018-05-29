#pragma once

#include "ContactCenter.h"

namespace gengeopop {

class Household : public ContactCenter
{
public:
        explicit Household(unsigned int id);
        Household();
        std::string  GetType() const override;
        unsigned int GetMaxPools() const override;
        unsigned int GetPoolSize() const override;
        void         Fill(const std::shared_ptr<GeoGrid>& geoGrid) override;
};

} // namespace gengeopop
