#include "GeoGridGenerator.h"
#include <iostream>
#include <memory>
#include <utility>
namespace gengeopop {

GeoGridGenerator::GeoGridGenerator(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid)
    : m_partialGenerators(), m_geoGrid(std::move(geoGrid)), m_geoGridConfig(geoGridConfig)
{
}

void GeoGridGenerator::GenerateGeoGrid()
{
        for (std::shared_ptr<PartialGenerator>& partialGen : m_partialGenerators) {
                partialGen->Apply(m_geoGrid, m_geoGridConfig);
        }
}

std::shared_ptr<GeoGrid> GeoGridGenerator::GetGeoGrid() { return m_geoGrid; }

void GeoGridGenerator::AddPartialGenerator(std::shared_ptr<PartialGenerator> gen)
{
        m_partialGenerators.push_back(gen);
}

} // namespace gengeopop
