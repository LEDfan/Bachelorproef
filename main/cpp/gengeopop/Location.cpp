#include "Location.h"

namespace gengeopop {
    std::string Location::getName(){
        return m_name;
    }

    unsigned int Location::getProvince(){
        return m_province;
    }

    unsigned int Location::getID(){
        return m_id;
    }
}

