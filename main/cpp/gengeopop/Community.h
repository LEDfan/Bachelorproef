#pragma once

#include "ContactCenter.h"
#include "Household.h"

namespace gengeopop {

    class Community : public ContactCenter {
    public:
        void addHouseHold(std::shared_ptr<Household> household);

    };

}

