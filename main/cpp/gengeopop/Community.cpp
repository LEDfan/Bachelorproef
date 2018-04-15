#include "Community.h"
#include "GeoGridConfig.h"
#include <Exception.h>
namespace gengeopop {

Community::Community(unsigned int id) : ContactCenter(id) {}

std::string  Community::getType() const { return "Community"; }
unsigned int Community::getPoolSize() const { return 2000; }
unsigned int Community::getMaxPools() const { return 1; }
void         Community::fill(GeoGridConfig& geoGridConfig)
{
        if (m_pools.empty()) {
                m_pools.push_back(
                    std::make_shared<ContactPool>(geoGridConfig.generated.contactPools++, this->getPoolSize()));
        }
}

void Community::addHouseHold(std::shared_ptr<Household> household)
{
        if (m_pools.empty()) {
                throw Exception("Could not add the Household to the Community, no ContactPool available");
        } else {
                if (household->GetPools().empty())
                        return;
                for (const std::shared_ptr<stride::Person>& person : **household->begin()) {
                        m_pools[0]->addMember(person);
                }
        }
}
bool Community::isAvailable() const
{
        if (m_pools.empty()) {
                return false;
        } else {
                return m_pools[0]->getCapacity() > m_pools[0]->getUsedCapacity();
        }
}
} // namespace gengeopop
