#include "School.h"

namespace gengeopop {

School::School(unsigned int id) : ContactCenter(id) {}

unsigned int School::getMaxPools() const { return 25; }

unsigned int School::getPoolSize() const { return 20; }

std::string School::getType() const { return "School"; }

} // namespace gengeopop
