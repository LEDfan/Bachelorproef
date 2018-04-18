#pragma once

#include "../GeoGrid.h"
#include <memory>
#include <ostream>

namespace gengeopop {

class GeoGridReader
{
public:
        GeoGridReader();
        virtual ~GeoGridReader()                                    = default;
        virtual std::shared_ptr<GeoGrid> Read(std::istream &stream) = 0;

protected:
        void AddSubMunicipalities(std::shared_ptr<GeoGrid> geoGrid);
        void AddCommutes(std::shared_ptr<GeoGrid> geoGrid);

        std::map<unsigned int, std::shared_ptr<stride::Person>>     m_people;
        std::vector<std::tuple<unsigned int, unsigned int, double>> m_commutes;          ///< from, to, amount
        std::vector<std::pair<unsigned int, unsigned int>>          m_subMunicipalities; //// first has submun second
};

} // namespace gengeopop
