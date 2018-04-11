#pragma once

#include "Community.h"
#include "Household.h"

namespace gengeopop {

class PrimaryCommunity : public Community
{
public:
        PrimaryCommunity(unsigned int id);
        std::string getType() const;
};

} // namespace gengeopop
