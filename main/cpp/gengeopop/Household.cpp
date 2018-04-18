#include "Household.h"
#include "GeoGridConfig.h"

namespace gengeopop {

Household::Household() : ContactCenter(0) {}

Household::Household(unsigned int id) : ContactCenter(id) {}

unsigned int Household::GetMaxPools() const { return 1; }

unsigned int Household::GetPoolSize() const { return 15; }

std::string Household::GetType() const { return "Household"; }

void Household::Fill(GeoGridConfig &geoGridConfig)
{
        AddPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, GetPoolSize()));
}
} // namespace gengeopop
