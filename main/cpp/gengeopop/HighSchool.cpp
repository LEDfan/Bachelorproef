#include "HighSchool.h"

namespace gengeopop {

HighSchool::HighSchool(unsigned int id) : ContactCenter(id) {}

std::string HighSchool::getType() const { return "HighSchool"; }

unsigned int HighSchool::getMaxPools() const
{
        return 0; // TODO
}

unsigned int HighSchool::getPoolSize() const
{
        return 0; // TODO
}
} // namespace gengeopop
