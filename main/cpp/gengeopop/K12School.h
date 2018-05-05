#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class K12School : public ContactCenter
{
public:
        explicit K12School(unsigned int id);
        std::string  GetType() const override;
        unsigned int GetMaxPools() const override;
        unsigned int GetPoolSize() const override;
        void         Fill(GeoGridConfig& geoGridConfig) override;
};
} // namespace gengeopop
