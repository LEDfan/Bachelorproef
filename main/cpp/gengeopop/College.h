#pragma once

#include "ContactCenter.h"
#include "GeoGrid.h"

namespace gengeopop {
/// Models a College as ContactCenter
class College : public ContactCenter
{
public:
        explicit College(unsigned int id);
        std::string  GetType() const override;
        unsigned int GetPoolSize() const override;
        unsigned int GetMaxPools() const override;
        void         Fill(const std::shared_ptr<GeoGrid>& geoGrid) override;
};
} // namespace gengeopop
