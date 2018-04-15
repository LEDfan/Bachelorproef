#pragma once

#include "ContactCenter.h"
#include "Household.h"

namespace gengeopop {

class Community : public ContactCenter
{
public:
        explicit Community(unsigned int id);
        void         addHouseHold(std::shared_ptr<Household> household);
        std::string  getType() const override;
        bool         isAvailable() const override;
        unsigned int getPoolSize() const override;
        unsigned int getMaxPools() const override;
        void         fill(GeoGridConfig& geoGridConfig) override;
};

} // namespace gengeopop
