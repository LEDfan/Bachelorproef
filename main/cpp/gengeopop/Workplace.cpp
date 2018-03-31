#include "Workplace.h"

namespace gengeopop {

Workplace::Workplace(unsigned int id) : ContactCenter(id) {}

std::string Workplace::getType() const { return "Workplace"; }

unsigned int Workplace::getPoolSize() const { return 20; }

unsigned int Workplace::getMaxPools() const { return 0; }

} // namespace gengeopop
