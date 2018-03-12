#pragma once

#include <string>
#include <vector>
#include "ContactCenter.h"
#include "Coordinate.h"

namespace gengeopop {
    using LocationIterator = std::vector<std::shared_ptr<ContactCenter>>::iterator;
    class Location {
    public:
        Location(unsigned int id, unsigned int province, unsigned int population = 0, Coordinate coordinate = Coordinate(0.0,0.0,0.0,0.0), std::string name = "");

        void addContactCenter(std::shared_ptr<ContactCenter> center);
        std::string getName();
        unsigned int getProvince();
        unsigned int getID();
        unsigned int getPopulation();
        Coordinate& getCoordinate();

        LocationIterator begin();
        LocationIterator end();

        const std::vector<std::shared_ptr<ContactCenter> >& getContactCenters() const;


    private:
        unsigned int m_id = 0;
        std::string m_name;
        unsigned int m_province;
        unsigned int m_population;
        Coordinate m_coordinate;
        std::vector<std::shared_ptr<ContactCenter> >  m_contactCenters;

    };

}
