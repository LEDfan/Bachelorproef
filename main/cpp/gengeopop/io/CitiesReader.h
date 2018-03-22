#pragma once

#include <gengeopop/GeoGrid.h>
#include <gengeopop/Location.h>
#include <map>
#include <string>

namespace gengeopop {
class CitiesReader
{
public:
        CitiesReader(std::unique_ptr<std::istream> inputStream);

        virtual void FillGeoGrid(std::shared_ptr<GeoGrid>) = 0;

        unsigned int getTotalPopulation() const;

        virtual ~CitiesReader(){};

protected:
        std::unique_ptr<std::istream> m_inputStream;
        unsigned int                  m_totalPopulation;
};
} // namespace gengeopop
