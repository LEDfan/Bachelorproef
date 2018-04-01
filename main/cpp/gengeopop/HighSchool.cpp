#include "HighSchool.h"
#include "GeoGridConfig.h"

namespace gengeopop {

HighSchool::HighSchool(unsigned int id) : ContactCenter(id) {}

std::string HighSchool::getType() const { return "HighSchool"; }

unsigned int HighSchool::getMaxPools() const { return 20; }

unsigned int HighSchool::getPoolSize() const
{
        return 0; // TODO
}

void HighSchool::fill(GeoGridConfig& geoGridConfig)
{
        for (std::size_t i = 0; i < getMaxPools(); ++i) {
                addPool(std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, getPoolSize()));
        }
}
} // namespace gengeopop
