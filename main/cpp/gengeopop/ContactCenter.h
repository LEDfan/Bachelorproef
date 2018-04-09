#pragma once
#include "ContactPool.h"

namespace gengeopop {

class GeoGridConfig;

class ContactCenter
{
public:
        using iterator = std::vector<std::shared_ptr<ContactPool>>::iterator;

        ContactCenter(unsigned int id);

        virtual std::string  getType() const     = 0;
        virtual unsigned int getPoolSize() const = 0;
        virtual unsigned int getMaxPools() const = 0;
        virtual bool         isAvailable() const;
        virtual void         fill(GeoGridConfig& geoGridConfig) = 0;

        unsigned int getId() const;

        void addPool(const std::shared_ptr<ContactPool>& pool);

        const std::vector<std::shared_ptr<ContactPool>>& GetPools() const;

        iterator begin();
        iterator end();

        virtual ~ContactCenter(){};

protected:
        std::vector<std::shared_ptr<ContactPool>> m_pools;
        unsigned int                              m_id;
};

} // namespace gengeopop
