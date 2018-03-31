#include "Community.h"

namespace gengeopop {

Community::Community(unsigned int id) : ContactCenter(id) {}

std::string  Community::getType() const { return "Community"; }
unsigned int Community::getPoolSize() const
{
        return 0; // TOOD
}
unsigned int Community::getMaxPools() const
{
        return 0; // TODO
}

} // namespace gengeopop
