#pragma once
#include "ContactPool.h"


namespace gengeopop {
    using ContactCenterIterator = std::vector<std::shared_ptr<ContactPool>>::iterator;
    class ContactCenter {
    public:
        virtual std::string getType()=0;
        virtual unsigned int getPoolSize() const = 0;
        virtual unsigned int getMaxPools() const = 0;

        void addPool(std::shared_ptr<ContactPool> pool);

        ContactCenterIterator begin();
        ContactCenterIterator end();
    private:
        std::vector<std::shared_ptr<ContactPool> > m_pools;
        unsigned int m_max_pools;


    };

}
