#include "SecondaryCommunity.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

SecondaryCommunity::SecondaryCommunity(unsigned int id) : Community(id) {}

std::string SecondaryCommunity::GetType() const { return "Secondary Community"; }

void SecondaryCommunity::Fill(const std::shared_ptr<GeoGrid>& geoGrid)
{
        if (m_pools.empty()) {
                AddPool(geoGrid->CreateContactPool(Id::SecondaryCommunity));
        }
}

} // namespace gengeopop
