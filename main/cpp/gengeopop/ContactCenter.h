#pragma once
#include "ContactPool.h"

namespace gengeopop {

class GeoGridConfig;

class ContactCenter
{
public:
        using iterator = std::vector<std::shared_ptr<ContactPool>>::iterator;

        explicit ContactCenter(unsigned int id);

        virtual std::string  GetType() const                    = 0;
        virtual unsigned int GetPoolSize() const                = 0;
        virtual unsigned int GetMaxPools() const                = 0;
        virtual void         Fill(GeoGridConfig& geoGridConfig) = 0;

        unsigned int GetId() const;

        std::pair<unsigned int, unsigned int> GetPopulationAndInfectedCount() const;

        void AddPool(std::shared_ptr<ContactPool> pool);

        const std::vector<std::shared_ptr<ContactPool>>& GetPools() const;

        iterator begin();
        iterator end();

        virtual ~ContactCenter() = default;

protected:
        std::vector<std::shared_ptr<ContactPool>> m_pools;
        unsigned int                              m_id;
};

} // namespace gengeopop
