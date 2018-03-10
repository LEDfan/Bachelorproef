#include "GeoGridGenerator.h"
#include <memory>

namespace gengeopop {
    std::shared_ptr<gengeopop::GeoGrid> GeoGridGenerator::generateGeoGrid(std::string filename) {
            for (std::shared_ptr<IPartialGenerator> &partialGen : m_partialGenerators) {
                partialGen->apply(m_geoGrid);
            }
            return m_geoGrid;
    }

    void GeoGridGenerator::addPartialGenerator(std::shared_ptr<IPartialGenerator> gen) {
            m_partialGenerators.push_back(gen);
    }

}

