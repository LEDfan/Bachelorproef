#include "School.h"
#include "GeoGridConfig.h"

namespace gengeopop {

School::School(unsigned int id) : ContactCenter(id) {}

unsigned int School::getMaxPools() const { return 25; }

unsigned int School::getPoolSize() const { return 20; }

std::string School::getType() const { return "School"; }

void School::fill(GeoGridConfig& geoGridConfig)
{
        for (std::size_t i = 0; i < getMaxPools(); ++i) {
                addPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, getPoolSize()));
        }
}

} // namespace gengeopop
