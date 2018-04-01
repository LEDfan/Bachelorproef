#include "Workplace.h"
#include "GeoGridConfig.h"

namespace gengeopop {

Workplace::Workplace(unsigned int id) : ContactCenter(id) {}

std::string Workplace::getType() const { return "Workplace"; }

unsigned int Workplace::getPoolSize() const { return 20; }

unsigned int Workplace::getMaxPools() const { return 1; }

void Workplace::fill(GeoGridConfig& geoGridConfig)
{
        addPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, getPoolSize()));
}

} // namespace gengeopop
