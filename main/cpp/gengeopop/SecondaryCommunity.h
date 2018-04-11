#pragma once

#include "Community.h"

namespace gengeopop {

class SecondaryCommunity : public Community
{
public:
        SecondaryCommunity(unsigned int id);
        std::string getType() const;
};

} // namespace gengeopop
