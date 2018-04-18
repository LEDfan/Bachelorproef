#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class School : public ContactCenter
{
public:
        explicit School(unsigned int id);
        std::string  GetType() const override;
        unsigned int GetMaxPools() const override;
        unsigned int GetPoolSize() const override;
        void         Fill(GeoGridConfig &geoGridConfig) override;
};
} // namespace gengeopop
