#include "College.h"
#include "GeoGridConfig.h"

namespace gengeopop {

College::College(unsigned int id) : ContactCenter(id) {}

std::string College::GetType() const { return "College"; }

unsigned int College::GetMaxPools() const { return 20; }

unsigned int College::GetPoolSize() const
{
        return 0; // TODO
}

void College::Fill(GeoGridConfig& geoGridConfig)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, GetPoolSize()));
        }
}
} // namespace gengeopop
