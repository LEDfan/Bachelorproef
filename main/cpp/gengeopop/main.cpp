#include "Community.h"
#include "GeoGrid.h"
#include "GeoGridConfig.h"
#include <tclap/CmdLine.h>
#include <gengeopop/generators/CommunityGenerator.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/generators/HighSchoolGenerator.h>
#include <gengeopop/generators/HouseholdGenerator.h>
#include <gengeopop/generators/SchoolGenerator.h>
#include <gengeopop/generators/WorkplaceGenerator.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <gengeopop/io/ReaderFactory.h>

#include <boost/lexical_cast.hpp>
#include <gengeopop/populators/GeoGridPopulator.h>
#include <gengeopop/populators/HighSchoolPopulator.h>
#include <gengeopop/populators/HouseholdPopulator.h>
#include <gengeopop/populators/PrimaryCommunityPopulator.h>
#include <gengeopop/populators/SchoolPopulator.h>
#include <gengeopop/populators/SecondaryCommunityPopulator.h>
#include <gengeopop/populators/WorkplacePopulator.h>

using namespace gengeopop;
using namespace TCLAP;

void genGeo(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid, stride::util::RNManager& rnManager)
{
        GeoGridGenerator geoGridGenerator(geoGridConfig, geoGrid);
        geoGridGenerator.addPartialGenerator(std::make_shared<SchoolGenerator>(rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<HighSchoolGenerator>(rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<WorkplaceGenerator>(rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<CommunityGenerator>(rnManager));
        geoGridGenerator.generateGeoGrid();
}

void genPop(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid, stride::util::RNManager& rnManager)
{
        // TODO move to genGeo
        GeoGridGenerator geoGridGenerator(geoGridConfig, geoGrid);
        geoGridGenerator.addPartialGenerator(std::make_shared<HouseholdGenerator>(rnManager));
        geoGridGenerator.generateGeoGrid();

        GeoGridPopulator geoGridPopulator(geoGridConfig, geoGrid);
        geoGridPopulator.addPartialPopulator(std::make_shared<HouseholdPopulator>(rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<SchoolPopulator>(rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<HighSchoolPopulator>(rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<PrimaryCommunityPopulator>(rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<SecondaryCommunityPopulator>(rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<WorkplacePopulator>(rnManager));
        geoGridPopulator.populateGeoGrid();
}

void generate(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid)
{
        stride::util::RNManager::Info info;
        stride::util::RNManager       rnManager(info);
        genGeo(geoGridConfig, geoGrid, rnManager);
        genPop(geoGridConfig, geoGrid, rnManager);
}

int main(int argc, char* argv[])
{
        int exit_status = EXIT_SUCCESS;

        // base structure copied from sim/main.cpp
        try {
                CmdLine               cmd("gengeopop", ' ', "1.0");
                ValueArg<std::string> citiesFile("c", "cities", "Cities File", false, "flanders_cities.csv",
                                                 "CITIES FILE", cmd);
                ValueArg<std::string> commutingFile("m", "commuting", "Commuting File", false, "flanders_commuting.csv",
                                                    "COMMUTING FILE", cmd);
                ValueArg<std::string> outputFile("o", "output", "Output File", false, "gengeopop.json", "OUTPUT FILE",
                                                 cmd);
                ValueArg<std::string> houseHoldFile("u", "household", "Household File", false,
                                                    "households_flanders.csv", "OUTPUT FILE", cmd);
                ValueArg<double>      fraction1826Students("s", "frac1826students",
                                                      "Fraction of 1826 years which are students", false, 0.50,
                                                      "FRACTION STUDENTS (1826)", cmd);

                ValueArg<double> fractionCommutingPeople("t", "fracCommuting", "Fraction of people commuting", false,
                                                         0.50, "FRACTION OF PEOPLE COMMUTING", cmd);

                ValueArg<unsigned int> populationSize("p", "populationSize", "Population size", false, 1000000,
                                                      "POPULATION SIZE", cmd);

                cmd.parse(argc, static_cast<const char* const*>(argv));

                ReaderFactory readerFactory;

                std::shared_ptr<CitiesReader>    citiesReader;
                std::shared_ptr<CommutesReader>  commutesReader;
                std::shared_ptr<HouseholdReader> houseHoldsReader;
                auto                             geoGrid = std::make_shared<GeoGrid>();

#pragma omp parallel sections
                {
#pragma omp section
                        {
                                citiesReader = readerFactory.CreateCitiesReader(std::string(citiesFile.getValue()));
                                citiesReader->FillGeoGrid(geoGrid);
                        }

#pragma omp section
                        {
                                commutesReader =
                                    readerFactory.CreateCommutesReader(std::string(commutingFile.getValue()));
                        }

#pragma omp section
                        {
                                houseHoldsReader =
                                    readerFactory.CreateHouseholdReader(std::string(houseHoldFile.getValue()));
                        }
                }

                std::ofstream outputFileStream(outputFile.getValue());
                auto          subMunicipalitiesReader =
                    readerFactory.CreateSubMunicipalitiesReader(std::string(subMunicipalitiesFile.getValue()));

                citiesReader->FillGeoGrid(geoGrid);
                commutesReader->FillGeoGrid(geoGrid); // TODO
                subMunicipalitiesReader->FillGeoGrid(geoGrid);

                GeoGridConfig geoGridConfig{};
                geoGridConfig.input.populationSize                       = populationSize.getValue();
                geoGridConfig.input.fraction_1826_years_WhichAreStudents = fraction1826Students.getValue();

                geoGridConfig.Calculate(geoGrid, houseHoldsReader);
                geoGrid->finalize();

                std::cout << geoGridConfig;

                stride::util::RNManager::Info info;
                stride::util::RNManager       rnManager(info);
                std::cout << "Starting Gen-Geo" << std::endl;
                // FIXME use this instead once I/O is fixed
                // generate(geoGridConfig, geoGrid);
                genGeo(geoGridConfig, geoGrid, rnManager);
                std::cout << "ContactCenters generated: " << geoGridConfig.generated.contactCenters << std::endl;
                std::cout << "ContactPools generated: " << geoGridConfig.generated.contactPools << std::endl;
                std::cout << "Generation done, writing to file" << std::endl;
                GeoGridJSONWriter geoGridJsonWriter;
                geoGridJsonWriter.write(geoGrid, outputFileStream);
                std::cout << "Done writing to file, starting Gen-Pop" << std::endl;
                genPop(geoGridConfig, geoGrid, rnManager);

                std::cout << "Done" << std::endl;
        } catch (std::exception& e) {
                exit_status = EXIT_FAILURE;
                std::cerr << "\nEXCEPION THROWN: " << e.what() << std::endl;
        } catch (...) {
                exit_status = EXIT_FAILURE;
                std::cerr << "\nEXCEPION THROWN: Unknown exception." << std::endl;
        }
        return exit_status;
}
