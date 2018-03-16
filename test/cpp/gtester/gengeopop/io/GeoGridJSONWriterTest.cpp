#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <gtest/gtest.h>
#include <sstream>
#include <util/FileSys.h>

using namespace gengeopop;

namespace {
TEST(GeoGridJSONWriterTest, locationTest)
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
        std::stringstream ss;
        writer.write(geoGrid, ss);

        boost::property_tree::ptree result;
        boost::property_tree::read_json(ss, result);
        boost::property_tree::ptree expected;
        boost::property_tree::read_json(
            stride::util::FileSys::GetTestsDir().string() + "/testdata/GeoGridJSON/test0.json", expected);

        EXPECT_EQ(result, expected);
}
} // namespace
