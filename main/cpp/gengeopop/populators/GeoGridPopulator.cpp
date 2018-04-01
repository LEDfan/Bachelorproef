#include "GeoGridPopulator.h"
#include <iostream>
#include <memory>

namespace gengeopop {

// TODO geogrid copied!
GeoGridPopulator::GeoGridPopulator(GeoGridConfig geoGridConfig, std::shared_ptr<GeoGrid> geoGrid)
    : m_partialPopulators(), m_geoGrid(geoGrid), m_geoGridConfig(geoGridConfig)
{
}

void GeoGridPopulator::populateGeoGrid()
{
        for (std::shared_ptr<PartialPopulator>& partialGen : m_partialPopulators) {
                partialGen->apply(m_geoGrid, m_geoGridConfig);
        }
}

std::shared_ptr<GeoGrid> GeoGridPopulator::getGeoGrid() { return m_geoGrid; }

void GeoGridPopulator::addPartialPopulator(std::shared_ptr<PartialPopulator> gen)
{
        m_partialPopulators.push_back(gen);
}

} // namespace gengeopop
