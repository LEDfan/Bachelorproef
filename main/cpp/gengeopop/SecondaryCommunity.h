#pragma once

#include "Community.h"

namespace gengeopop {

class SecondaryCommunity : public Community
{
public:
        explicit SecondaryCommunity(unsigned int id);
        std::string GetType() const override;
};

} // namespace gengeopop
