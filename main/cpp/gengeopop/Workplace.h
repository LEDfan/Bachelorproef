#pragma once
#include "ContactCenter.h"

namespace gengeopop {

class Workplace : public ContactCenter
{
public:
        explicit Workplace(unsigned int id);
        unsigned int GetPoolSize() const override;
        unsigned int GetMaxPools() const override;
        std::string  GetType() const override;
        void         Fill(GeoGridConfig &geoGridConfig) override;
};
} // namespace gengeopop
