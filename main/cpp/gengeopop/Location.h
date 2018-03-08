#pragma once

#include <string>
#include <vector>
#include "ContactCenter.h"

namespace gengeopop {
    class Location {
    public:
        void addContactCenter(std::shared_ptr<ContactCenter> center);

        std::string getName();
        unsigned int getProvince();
        unsigned int getID();

    private:
        unsigned int m_id = 0;
        std::string m_name;
        unsigned int m_province;
        std::vector<std::shared_ptr<ContactCenter> >  m_contactCenters;

    };

}
