#pragma once
#include "GeoGrid.h"

namespace gengeopop {

class GeoGridConfig;

class ContactCenter
{
public:
        using iterator = std::vector<stride::ContactPool*>::iterator;

        explicit ContactCenter(unsigned int id);

        virtual std::string  GetType() const                               = 0;
        virtual unsigned int GetPoolSize() const                           = 0;
        virtual unsigned int GetMaxPools() const                           = 0;
        virtual void         Fill(const std::shared_ptr<GeoGrid>& geoGrid) = 0;

        unsigned int GetId() const;

        std::pair<unsigned int, unsigned int> GetPopulationAndInfectedCount() const;

        void AddPool(stride::ContactPool* pool);

        const std::vector<stride::ContactPool*>& GetPools() const;

        iterator begin();
        iterator end();

        virtual ~ContactCenter() = default;

protected:
        std::vector<stride::ContactPool*> m_pools;
        unsigned int                      m_id;
};

} // namespace gengeopop
