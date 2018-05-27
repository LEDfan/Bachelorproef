#include "Workplace.h"
#include "GeoGridConfig.h"

using namespace stride::ContactPoolType;

namespace gengeopop {

Workplace::Workplace(unsigned int id) : ContactCenter(id) {}

std::string Workplace::GetType() const { return "Workplace"; }

unsigned int Workplace::GetPoolSize() const { return 20; }

unsigned int Workplace::GetMaxPools() const { return 1; }

void Workplace::Fill(const std::shared_ptr<GeoGrid>& geoGrid) { AddPool(geoGrid->CreateContactPool(Id::Work)); }

} // namespace gengeopop
