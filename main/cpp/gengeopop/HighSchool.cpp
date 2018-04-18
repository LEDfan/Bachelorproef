#include "HighSchool.h"
#include "GeoGridConfig.h"

namespace gengeopop {

HighSchool::HighSchool(unsigned int id) : ContactCenter(id) {}

std::string HighSchool::GetType() const { return "HighSchool"; }

unsigned int HighSchool::GetMaxPools() const { return 20; }

unsigned int HighSchool::GetPoolSize() const
{
        return 0; // TODO
}

void HighSchool::Fill(GeoGridConfig &geoGridConfig)
{
        for (std::size_t i = 0; i < GetMaxPools(); ++i) {
                AddPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, GetPoolSize()));
        }
}
} // namespace gengeopop
