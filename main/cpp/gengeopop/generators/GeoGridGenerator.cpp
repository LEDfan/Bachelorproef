#include "GeoGridGenerator.h"
#include <memory>

namespace gengeopop {

    GeoGridGenerator::GeoGridGenerator(GeoGridConfig geoGridConfig) : m_geoGridConfig(geoGridConfig) {
    
        m_geoGrid = std::make_shared<GeoGrid>();
    }

    void GeoGridGenerator::generateGeoGrid() {
            for (std::shared_ptr<IPartialGenerator> &partialGen : m_partialGenerators) {
                partialGen->apply(m_geoGrid, m_geoGridConfig);
            }
    }

    std::shared_ptr<GeoGrid> GeoGridGenerator::getGeoGrid() {
        return m_geoGrid;
    }

    void GeoGridGenerator::addPartialGenerator(std::shared_ptr<IPartialGenerator> gen) {
            m_partialGenerators.push_back(gen);
    }

}

