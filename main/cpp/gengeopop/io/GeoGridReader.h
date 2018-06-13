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
        virtual std::shared_ptr<GeoGrid> Read() = 0;

        void UsePopulation(const std::shared_ptr<stride::Population>& pop, std::size_t regionId, std::string regionName)
        {
                m_population = pop;
                m_regionId   = regionId;
                m_regionName = std::move(regionName);
        }

protected:
        void AddSubMunicipalities(std::shared_ptr<GeoGrid> geoGrid);
        void AddCommutes(std::shared_ptr<GeoGrid> geoGrid);

        std::map<unsigned int, stride::Person*>                     m_people;
        std::vector<std::tuple<unsigned int, unsigned int, double>> m_commutes;          ///< from, to, amount
        std::vector<std::pair<unsigned int, unsigned int>>          m_subMunicipalities; //// first has submun second
        std::size_t                                                 m_regionId; ///< RegionId, used to create persons
        std::string m_regionName; ///< RegionName, region name passed to GeoGrid

        std::unique_ptr<std::istream> m_inputStream; ///< file to read

        std::shared_ptr<stride::Population> m_population{nullptr}; ///< population to use in the GeoGrid may be nullptr
};

} // namespace gengeopop
