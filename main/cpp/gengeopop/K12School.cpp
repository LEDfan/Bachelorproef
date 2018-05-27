#include "K12School.h"
#include "GeoGridConfig.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

K12School::K12School(unsigned int id) : ContactCenter(id) {}

unsigned int K12School::GetMaxPools() const { return 25; }

unsigned int K12School::GetPoolSize() const { return 20; }

std::string K12School::GetType() const { return "K12School"; }

void K12School::Fill(const std::shared_ptr<GeoGrid>& geoGrid)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(geoGrid->CreateContactPool(Id::School));
        }
}

} // namespace gengeopop
