#include "GeoGrid.h"
#include "generators/GeoGridGenerator.h"
#include "generators/SchoolGenerator.h"
#include "io/GeoGridJSONWriter.h"
#include <iostream>
#include <memory>

using namespace gengeopop;

int main()
{
        GeoGridConfig config;
        config.populationSize            = 10000;
        config.fraction_compulsoryPupils = 0.20;

        GeoGridGenerator         geoGridGenerator(config);
        std::shared_ptr<GeoGrid> geoGrid = geoGridGenerator.getGeoGrid();
        geoGrid->addLocation(std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove"));
        geoGrid->addLocation(std::make_shared<Location>(2, 3, 5000, Coordinate(0, 0, 0, 0), "Gent"));
        geoGrid->addLocation(std::make_shared<Location>(3, 2, 2500, Coordinate(0, 0, 0, 0), "Mons"));

        GeoGridJSONWriter writer;
        writer.write(geoGrid, std::cout);

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 100;
        stride::util::RNManager rnManager(rnInfo);

        auto schoolGenerator = std::make_shared<SchoolGenerator>(rnManager);

        geoGridGenerator.addPartialGenerator(schoolGenerator);

        geoGridGenerator.generateGeoGrid();

        return 0;
}
