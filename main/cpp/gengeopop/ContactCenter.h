#pragma once
#include "ContactPool.h"


namespace gengeopop {
    class ContactCenter {
    public:
        using iterator = std::vector<std::shared_ptr<ContactPool>>::iterator;

            ContactCenter();

        virtual std::string getType() const = 0;
        virtual unsigned int getPoolSize() const = 0;
        virtual unsigned int getMaxPools() const = 0;

        void addPool(std::shared_ptr<ContactPool> pool);

        iterator begin();
        iterator end();

        virtual ~ContactCenter() {};
    private:
        std::vector<std::shared_ptr<ContactPool> > m_pools;


    };

}
