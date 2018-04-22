#include "School.h"
#include "GeoGridConfig.h"

namespace gengeopop {

School::School(unsigned int id) : ContactCenter(id) {}

unsigned int School::GetMaxPools() const { return 25; }

unsigned int School::GetPoolSize() const { return 20; }

std::string School::GetType() const { return "School"; }

void School::Fill(GeoGridConfig& geoGridConfig)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, GetPoolSize()));
        }
}

} // namespace gengeopop
