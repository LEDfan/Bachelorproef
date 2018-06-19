#pragma once

#include "ContactCenter.h"
#include "GeoGrid.h"

namespace gengeopop {

/// Models a K12School as ContactCenter
class K12School : public ContactCenter
{
public:
        explicit K12School(unsigned int id);
        std::string  GetType() const override;
        unsigned int GetMaxPools() const override;
        unsigned int GetPoolSize() const override;
        void         Fill(const std::shared_ptr<GeoGrid>& geoGrid) override;
};
} // namespace gengeopop
