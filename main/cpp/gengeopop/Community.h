#ifndef STRIDE_COMMUNITY_H
#define STRIDE_COMMUNITY_H

#include "ContactCenter.h"
#include "Household.h"

namespace gengeopop {

    class Community : public ContactCenter {
    public:
        void addHouseHold(std::shared_ptr<Household> household);

    };

}


#endif //STRIDE_COMMUNITY_H
