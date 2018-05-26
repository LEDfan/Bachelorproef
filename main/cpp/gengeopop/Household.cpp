#include "Household.h"
#include "GeoGridConfig.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

Household::Household() : ContactCenter(0) {}

Household::Household(unsigned int id) : ContactCenter(id) {}

unsigned int Household::GetMaxPools() const { return 1; }

unsigned int Household::GetPoolSize() const { return 15; }

std::string Household::GetType() const { return "Household"; }

void Household::Fill(const std::shared_ptr<GeoGrid>& geoGrid)
{
        AddPool(geoGrid->CreateContactPool(GetPoolSize(), Id::Household));
}
} // namespace gengeopop
