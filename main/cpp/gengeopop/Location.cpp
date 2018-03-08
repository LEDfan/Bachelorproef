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

    Location::Location(unsigned int id, unsigned int province, unsigned int population, double x_coord, double y_coord,
                       double latittude, double longitude, std::string name) : m_id(id), m_province(province), m_name(name) {

    }
}

