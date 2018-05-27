#pragma once

#include "ContactCenter.h"
#include "GeoGrid.h"
#include "Household.h"

namespace gengeopop {

class Community : public ContactCenter
{
public:
        explicit Community(unsigned int id);

        void         AddHouseHold(std::shared_ptr<Household> household);
        std::string  GetType() const override;
        unsigned int GetPoolSize() const override;
        unsigned int GetMaxPools() const override;
};

} // namespace gengeopop
