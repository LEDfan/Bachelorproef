#include "College.h"
#include "GeoGridConfig.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

College::College(unsigned int id) : ContactCenter(id) {}

std::string College::GetType() const { return "College"; }

unsigned int College::GetMaxPools() const { return 20; }

unsigned int College::GetPoolSize() const { return 150; }

void College::Fill(const std::shared_ptr<GeoGrid>& geoGrid)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(geoGrid->CreateContactPool(Id::College));
        }
}
} // namespace gengeopop
