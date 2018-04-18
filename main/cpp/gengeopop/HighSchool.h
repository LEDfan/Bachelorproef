#pragma once

#include "ContactCenter.h"

namespace gengeopop {
class HighSchool : public ContactCenter
{
public:
        explicit HighSchool(unsigned int id);
        std::string  GetType() const override;
        unsigned int GetPoolSize() const override;
        unsigned int GetMaxPools() const override;
        void         Fill(GeoGridConfig &geoGridConfig) override;
};
} // namespace gengeopop
