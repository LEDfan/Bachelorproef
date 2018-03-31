#pragma once

#include "PartialPopulator.h"

namespace gengeopop {
class GeoGridPopulator
{
public:
        GeoGridPopulator(GeoGridConfig geoGridConfig, std::shared_ptr<GeoGrid> geoGrid);
        void addPartialPopulator(std::shared_ptr<PartialPopulator> gen);

        /**
         * @param filename Filename with the config of the wanted geogrid.
         * @return The GeoGrid that was generated by applying all partial populators onto an empty GeoGrid object.
         */
        void populateGeoGrid();

        std::shared_ptr<GeoGrid> getGeoGrid();

private:
        std::vector<std::shared_ptr<PartialPopulator>> m_partialPopulators;
        std::shared_ptr<GeoGrid>                       m_geoGrid; // FIXME unique pointer?
        GeoGridConfig                                  m_geoGridConfig;
};
} // namespace gengeopop
