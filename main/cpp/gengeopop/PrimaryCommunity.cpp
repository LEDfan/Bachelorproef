#include "PrimaryCommunity.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

PrimaryCommunity::PrimaryCommunity(unsigned int id) : Community(id) {}
std::string PrimaryCommunity::GetType() const { return "Primary Community"; }

void PrimaryCommunity::Fill(const std::shared_ptr<GeoGrid>& geoGrid)
{
        if (m_pools.empty()) {
                AddPool(geoGrid->CreateContactPool(GetPoolSize(), Id::PrimaryCommunity));
        }
}

} // namespace gengeopop
