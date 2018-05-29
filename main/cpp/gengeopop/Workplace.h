#pragma once
#include "ContactCenter.h"
#include "GeoGrid.h"

namespace gengeopop {

class Workplace : public ContactCenter
{
public:
        explicit Workplace(unsigned int id);
        unsigned int GetPoolSize() const override;
        unsigned int GetMaxPools() const override;
        std::string  GetType() const override;
        void         Fill(const std::shared_ptr<GeoGrid>& geoGrid) override;
};
} // namespace gengeopop
