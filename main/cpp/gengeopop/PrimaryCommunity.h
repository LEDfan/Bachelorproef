#pragma once

#include "Community.h"
#include "Household.h"

namespace gengeopop {

class PrimaryCommunity : public Community
{
public:
        explicit PrimaryCommunity(unsigned int id);
        std::string GetType() const override;
        void        Fill(const std::shared_ptr<GeoGrid>& geoGrid) override;
};

} // namespace gengeopop
