#include "GeoGridPopulator.h"
#include <iostream>
#include <memory>

namespace gengeopop {

GeoGridPopulator::GeoGridPopulator(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid)
    : m_partialPopulators(), m_geoGrid(std::move(geoGrid)), m_geoGridConfig(geoGridConfig)
{
}

void GeoGridPopulator::PopulateGeoGrid()
{
        for (std::shared_ptr<PartialPopulator>& partialGen : m_partialPopulators) {
                partialGen->Apply(m_geoGrid, m_geoGridConfig);
        }
}

std::shared_ptr<GeoGrid> GeoGridPopulator::GetGeoGrid() { return m_geoGrid; }

void GeoGridPopulator::AddPartialPopulator(std::shared_ptr<PartialPopulator> gen)
{
        m_partialPopulators.push_back(gen);
}

} // namespace gengeopop
