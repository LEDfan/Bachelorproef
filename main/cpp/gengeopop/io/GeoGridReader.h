#pragma once

#include "../GeoGrid.h"
#include <memory>
#include <ostream>

namespace gengeopop {

class GeoGridReader
{
public:
        GeoGridReader(std::unique_ptr<std::istream> inputStream);
        virtual ~GeoGridReader()                = default;
        virtual std::shared_ptr<GeoGrid> read() = 0;

        void UsePopulation(const std::shared_ptr<stride::Population>& pop) { m_population = pop; }

protected:
        void addSubMunicipalities(std::shared_ptr<GeoGrid> geoGrid);
        void addCommutes(std::shared_ptr<GeoGrid> geoGrid);

        std::map<unsigned int, stride::Person*>                     m_people;
        std::vector<std::tuple<unsigned int, unsigned int, double>> m_commutes;          ///< from, to, amount
        std::vector<std::pair<unsigned int, unsigned int>>          m_subMunicipalities; //// first has submun second

        std::unique_ptr<std::istream> m_inputStream; ///< file to read

        std::shared_ptr<stride::Population> m_population{nullptr}; ///< population to use in the GeoGrid may be nullptr
};

} // namespace gengeopop
