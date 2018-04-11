#include "GeoGridGenerator.h"
#include <iostream>
#include <memory>

namespace gengeopop {

GeoGridGenerator::GeoGridGenerator(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid)
    : m_partialGenerators(), m_geoGrid(geoGrid), m_geoGridConfig(geoGridConfig)
{
}

void GeoGridGenerator::generateGeoGrid()
{
        for (std::shared_ptr<PartialGenerator>& partialGen : m_partialGenerators) {
                partialGen->apply(m_geoGrid, m_geoGridConfig);
        }
}

std::shared_ptr<GeoGrid> GeoGridGenerator::getGeoGrid() { return m_geoGrid; }

void GeoGridGenerator::addPartialGenerator(std::shared_ptr<PartialGenerator> gen)
{
        m_partialGenerators.push_back(gen);
}

} // namespace gengeopop
