#include "K12School.h"
#include "GeoGridConfig.h"

namespace gengeopop {

K12School::K12School(unsigned int id) : ContactCenter(id) {}

unsigned int K12School::GetMaxPools() const { return 25; }

unsigned int K12School::GetPoolSize() const { return 20; }

std::string K12School::GetType() const { return "K12School"; }

void K12School::Fill(GeoGridConfig& geoGridConfig)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, GetPoolSize()));
        }
}

} // namespace gengeopop
