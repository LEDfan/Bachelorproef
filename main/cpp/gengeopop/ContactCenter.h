#pragma once
#include "GeoGrid.h"

namespace gengeopop {

class GeoGridConfig;

/// A ContactCenter contains multiple ContactPools
class ContactCenter
{
public:
        /// Allow range-based iteration
        using iterator = std::vector<stride::ContactPool*>::iterator;

        /// Constructor
        explicit ContactCenter(unsigned int id);

        /// Get the name of the type of contact center (e.g. College)
        virtual std::string GetType() const = 0;

        /// Get the size of a pool
        virtual unsigned int GetPoolSize() const = 0;

        /// Get the maximal number of pools for this center
        virtual unsigned int GetMaxPools() const = 0;

        /// Apply this ContactCenter to the GeoGrid
        virtual void Fill(const std::shared_ptr<GeoGrid>& geoGrid) = 0;

        /// Get the ID back
        unsigned int GetId() const;

        /// Get a count of total population (first) and total number of infections (second)
        std::pair<unsigned int, unsigned int> GetPopulationAndInfectedCount() const;

        /// Add a ContactPool
        void AddPool(stride::ContactPool* pool);

        /// Get the pools
        const std::vector<stride::ContactPool*>& GetPools() const;

        /// Allow range-based iteration
        iterator begin();
        /// Allow range-based iteration
        iterator end();

        /// Default destructor, but virtual
        virtual ~ContactCenter() = default;

protected:
        std::vector<stride::ContactPool*> m_pools; ///< Our pools
        unsigned int                      m_id;    ///< The id of this ContactCenter
};

} // namespace gengeopop
