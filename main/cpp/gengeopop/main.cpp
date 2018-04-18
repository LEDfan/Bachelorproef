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
#include <gengeopop/io/GeoGridProtoWriter.h>
#include <gengeopop/io/GeoGridWriterFactory.h>
#include <gengeopop/io/ReaderFactory.h>

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <gengeopop/populators/GeoGridPopulator.h>
#include <gengeopop/populators/HighSchoolPopulator.h>
#include <gengeopop/populators/HouseholdPopulator.h>
#include <gengeopop/populators/PrimaryCommunityPopulator.h>
#include <gengeopop/populators/SchoolPopulator.h>
#include <gengeopop/populators/SecondaryCommunityPopulator.h>
#include <gengeopop/populators/WorkplacePopulator.h>
#include <iostream>
#include <utility>

using namespace gengeopop;
using namespace TCLAP;

void genGeo(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid, stride::util::RNManager& rnManager)
{
        GeoGridGenerator geoGridGenerator(geoGridConfig, std::move(geoGrid));
        geoGridGenerator.AddPartialGenerator(std::make_shared<SchoolGenerator>(rnManager));
        geoGridGenerator.AddPartialGenerator(std::make_shared<HighSchoolGenerator>(rnManager));
        geoGridGenerator.AddPartialGenerator(std::make_shared<WorkplaceGenerator>(rnManager));
        geoGridGenerator.AddPartialGenerator(std::make_shared<CommunityGenerator>(rnManager));
        geoGridGenerator.AddPartialGenerator(std::make_shared<HouseholdGenerator>(rnManager));
        geoGridGenerator.GenerateGeoGrid();
}

void genPop(GeoGridConfig& geoGridConfig, std::shared_ptr<GeoGrid> geoGrid, stride::util::RNManager& rnManager)
{
        GeoGridPopulator geoGridPopulator(geoGridConfig, std::move(geoGrid));
        geoGridPopulator.AddPartialPopulator(std::make_shared<HouseholdPopulator>(rnManager));
        geoGridPopulator.AddPartialPopulator(std::make_shared<SchoolPopulator>(rnManager));
        geoGridPopulator.AddPartialPopulator(std::make_shared<HighSchoolPopulator>(rnManager));
        geoGridPopulator.AddPartialPopulator(std::make_shared<PrimaryCommunityPopulator>(rnManager));
        geoGridPopulator.AddPartialPopulator(std::make_shared<SecondaryCommunityPopulator>(rnManager));
        geoGridPopulator.AddPartialPopulator(std::make_shared<WorkplacePopulator>(rnManager));
        geoGridPopulator.PopulateGeoGrid();
}

void generate(GeoGridConfig& geoGridConfig, const std::shared_ptr<GeoGrid>& geoGrid)
{
        stride::util::RNManager::Info info;
        stride::util::RNManager       rnManager(info);
        std::cout << "Starting Gen-Geo" << std::endl;
        genGeo(geoGridConfig, geoGrid, rnManager);
        std::cout << "Finished Gen-Geo" << std::endl;
        std::cout << "ContactCenters generated: " << geoGridConfig.generated.contactCenters << std::endl;
        std::cout << "ContactPools generated: " << geoGridConfig.generated.contactPools << std::endl;
        std::cout << "Starting Gen-Pop" << std::endl;
        genPop(geoGridConfig, geoGrid, rnManager);
        std::cout << "Finished Gen-Pop" << std::endl;
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
                ValueArg<std::string> outputFile("o", "output", "Output File", false, "gengeopop.proto", "OUTPUT FILE",
                                                 cmd);
                ValueArg<std::string> houseHoldFile("u", "household", "Household File", false,
                                                    "households_flanders.csv", "OUTPUT FILE", cmd);
                ValueArg<double>      fraction1826Students("s", "frac1826students",
                                                      "Fraction of 1826 years which are students", false, 0.50,
                                                      "FRACTION STUDENTS (1826)", cmd);

                ValueArg<double> fractionActiveCommutingPeople("t", "fracActiveCommuting",
                                                               "Fraction of active people commuting", false, 0.50,
                                                               "FRACTION OF ACTIVE PEOPLE COMMUTING", cmd);

                ValueArg<double> fractionStudentCommutingPeople("w", "fracStudentCommuting",
                                                                "Fraction of students commuting", false, 0.50,
                                                                "FRACTION OF STUDENTS COMMUTING", cmd);

                ValueArg<double> fractionActivePeople("a", "fracActive", "Fraction of people active", false, 1.00,
                                                      "FRACTION OF PEOPLE ACTIVE", cmd);

                ValueArg<unsigned int> populationSize("p", "populationSize", "Population size", false, 6000000,
                                                      "POPULATION SIZE", cmd);

                ValueArg<std::string> subMunicipalitiesFile("x", "subMinicipalities", "subMinicipalitiesFile", false,
                                                            "submunicipalities.csv", "OUTPUT FILE", cmd);

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
                commutesReader->FillGeoGrid(geoGrid);
                subMunicipalitiesReader->FillGeoGrid(geoGrid);

                GeoGridConfig geoGridConfig{};
                geoGridConfig.input.populationSize                       = populationSize.getValue();
                geoGridConfig.input.fraction_1826_years_WhichAreStudents = fraction1826Students.getValue();
                geoGridConfig.input.fraction_active_commutingPeople      = fractionActiveCommutingPeople.getValue();
                geoGridConfig.input.fraction_student_commutingPeople     = fractionStudentCommutingPeople.getValue();
                geoGridConfig.input.fraction_1865_years_active           = fractionActivePeople.getValue();

                geoGridConfig.Calculate(geoGrid, houseHoldsReader);
                geoGrid->Finalize();

                std::cout << geoGridConfig;

                stride::util::RNManager::Info info;
                stride::util::RNManager       rnManager(info);
                generate(geoGridConfig, geoGrid);
                std::cout << "Writing to file..." << std::endl;

                GeoGridWriterFactory           geoGridWriterFactory;
                std::shared_ptr<GeoGridWriter> geoGridWriter = geoGridWriterFactory.CreateWriter(outputFile.getValue());
                geoGridWriter->Write(geoGrid, outputFileStream);
                outputFileStream.close();

                std::cout << "Done writing to file" << std::endl;
        } catch (std::exception& e) {
                exit_status = EXIT_FAILURE;
                std::cerr << "\nEXCEPION THROWN: " << e.what() << std::endl;
        } catch (...) {
                exit_status = EXIT_FAILURE;
                std::cerr << "\nEXCEPION THROWN: Unknown exception." << std::endl;
        }
        return exit_status;
}
