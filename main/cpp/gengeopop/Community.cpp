#include "Community.h"
#include "GeoGridConfig.h"
#include <Exception.h>
namespace gengeopop {

Community::Community(unsigned int id) : ContactCenter(id) {}

std::string  Community::GetType() const { return "Community"; }
unsigned int Community::GetPoolSize() const { return 2000; }
unsigned int Community::GetMaxPools() const { return 1; }
void         Community::Fill(GeoGridConfig& geoGridConfig)
{
        if (m_pools.empty()) {
                m_pools.push_back(
                    std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, this->GetPoolSize()));
        }
}

void Community::AddHouseHold(std::shared_ptr<Household> household)
{
        if (m_pools.empty()) {
                throw Exception("Could not add the Household to the Community, no ContactPool available");
        } else {
                if (household->GetPools().empty())
                        return;
                for (stride::Person* person : **household->begin()) {
                        m_pools[0]->AddMember(person);
                }
        }
}
} // namespace gengeopop
