#include "Workplace.h"
#include "GeoGridConfig.h"

namespace gengeopop {

Workplace::Workplace(unsigned int id) : ContactCenter(id) {}

std::string Workplace::GetType() const { return "Workplace"; }

unsigned int Workplace::GetPoolSize() const { return 20; }

unsigned int Workplace::GetMaxPools() const { return 1; }

void Workplace::Fill(GeoGridConfig& geoGridConfig)
{
        AddPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, GetPoolSize()));
}

} // namespace gengeopop
