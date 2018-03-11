#pragma once

#include <string>
#include <vector>
#include "ContactCenter.h"

namespace gengeopop {
    class Location {
    public:
        Location(unsigned int id, unsigned int province, unsigned int population = 0, double x_coord = 0, double y_coord = 0, double latittude = 0, double longitude = 0, std::string name = "");

        void addContactCenter(std::shared_ptr<ContactCenter> center);

        std::string getName();
        unsigned int getProvince();
        unsigned int getID();
        unsigned int getPopulation();

    private:
        unsigned int m_id = 0;
        std::string m_name;
        unsigned int m_province;
        unsigned int m_population;
        std::vector<std::shared_ptr<ContactCenter> >  m_contactCenters;

    };

}
