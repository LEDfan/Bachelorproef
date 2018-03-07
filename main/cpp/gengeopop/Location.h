#pragma once

#include <string>
#include <vector>
#include "ContactCenter.h"

namespace gengeopop {
    class Location {
    public:
        void addContactCenter(std::shared_ptr<ContactCenter> center);


    private:
        unsigned int m_id = 0;
        std::string m_name;
        std::string m_province;
        std::vector<std::shared_ptr<ContactCenter> >  m_contactCenters;

    };

}
