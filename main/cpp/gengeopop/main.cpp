#include "GeoGrid.h"
#include "GeoGridConfig.h"
#include <tclap/CmdLine.h>
#include <gengeopop/generators/CommunityGenerator.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/generators/HighSchoolGenerator.h>
#include <gengeopop/generators/SchoolGenerator.h>
#include <gengeopop/generators/WorkplaceGenerator.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <gengeopop/io/ReaderFactory.h>
#include <util/StringUtils.h>

#include <boost/lexical_cast.hpp>
#include <utility>

using namespace gengeopop;
using namespace TCLAP;

void generate(GeoGridConfig geoGridConfig, std::shared_ptr<GeoGrid> geoGrid)
{
        stride::util::RNManager::Info info;

        stride::util::RNManager rnManager(info);

        GeoGridGenerator geoGridGenerator(geoGridConfig, std::move(geoGrid));
        geoGridGenerator.addPartialGenerator(std::make_shared<SchoolGenerator>(rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<HighSchoolGenerator>(rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<WorkplaceGenerator>(rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<CommunityGenerator>(rnManager));

        geoGridGenerator.generateGeoGrid();
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

                ValueArg<unsigned int> populationSize("p", "populationSize", "Population size", false, 6000000,
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

                commutesReader->FillGeoGrid(geoGrid);

                GeoGridConfig geoGridConfig{};
                geoGridConfig.input_populationSize                       = populationSize.getValue();
                geoGridConfig.input_fraction_1826_years_WhichAreStudents = fraction1826Students.getValue();
                geoGridConfig.input_fraction_commutingPeople             = fractionCommutingPeople.getValue();

                geoGridConfig.Calculate(geoGrid, houseHoldsReader);
                geoGrid->finalize();

                std::cout << geoGridConfig;

                generate(geoGridConfig, geoGrid);

                std::cout << "Generation done, writing to file." << std::endl;

                GeoGridJSONWriter geoGridJsonWriter;
                geoGridJsonWriter.write(geoGrid, outputFileStream);

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
