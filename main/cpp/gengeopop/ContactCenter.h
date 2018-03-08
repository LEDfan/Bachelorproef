#pragma once
#include "ContactPool.h"


namespace gengeopop {

    class ContactCenter {
    public:
        virtual unsigned int getPoolSize() = 0;
        virtual unsigned int getMaxPools() = 0;

        void addPool(std::shared_ptr<ContactPool> pool);

    private:
        std::vector<std::shared_ptr<ContactPool> > m_pools;
        unsigned int m_max_pools;


    };

}
