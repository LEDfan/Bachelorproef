#pragma once

#include "ContactCenter.h"
#include "Household.h"

namespace gengeopop {

class Community : public ContactCenter
{
public:
        Community(unsigned int id);
        void                addHouseHold(std::shared_ptr<Household> household);
        virtual std::string getType() const;
        bool                isAvailable() const;
        unsigned int        getPoolSize() const override;
        unsigned int        getMaxPools() const;
        void                fill(GeoGridConfig& geoGridConfig);
        virtual ~Community(){};
};

} // namespace gengeopop
