#include "GeoGridGenerator.h"

namespace gengeopop {
    std::shared_ptr<gengeopop::GeoGrid> GeoGridGenerator::generateGeoGrid(std::string filename) {
        // TODO
        return std::shared_ptr<GeoGrid>();
    }

    std::shared_ptr<GeoGrid> GeoGridGenerator::addPartialGenerator(std::shared_ptr<IPartialGenerator> gen) {
        GeoGrid* grid = new GeoGrid;

        for(IPartialGenerator& partialGen : m_partialGenerators) {
            partialGen.apply(*grid);
        }

        return std::shared_ptr<GeoGrid>(grid);

    }


}

