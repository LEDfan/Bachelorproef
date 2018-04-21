#include "Community.h"
#include "GenGeoPopController.h"
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
#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>
#include <util/LogUtils.h>
#include <utility>

using namespace gengeopop;
using namespace TCLAP;

int main(int argc, char* argv[])
{
        int exit_status = EXIT_SUCCESS;

        // base structure copied from sim/main.cpp
        try {
                // --------------------------------------------------------------
                // Parse parameters.
                // --------------------------------------------------------------
                CmdLine               cmd("gengeopop", ' ', "1.0");
                ValueArg<std::string> citiesFile("c", "cities", "Cities File", false, "flanders_cities.csv",
                                                 "CITIES FILE", cmd);
                ValueArg<std::string> commutingFile("m", "commuting", "Commuting File", false, "flanders_commuting.csv",
                                                    "COMMUTING FILE", cmd);
                ValueArg<std::string> outputFile("o", "output", "Output File", false, "gengeopop.proto", "OUTPUT FILE",
                                                 cmd);
                ValueArg<std::string> houseHoldFile("u", "household", "Household File", false,
                                                    "households_flanders.csv", "OUTPUT FILE", cmd);

                ValueArg<std::string> logLevel("l", "loglevel", "Loglevel", false, "info", "LOGLEVEL", cmd);

                ValueArg<double> fraction1826Students("s", "frac1826students",
                                                      "Fraction of 1826 years which are students", false, 0.50,
                                                      "FRACTION STUDENTS (1826)", cmd);

                ValueArg<double> fractionActiveCommutingPeople("t", "fracActiveCommuting",
                                                               "Fraction of active people commuting", false, 0.50,
                                                               "FRACTION OF ACTIVE PEOPLE COMMUTING", cmd);

                ValueArg<double> fractionStudentCommutingPeople("w", "fracStudentCommuting",
                                                                "Fraction of students commuting", false, 0.50,
                                                                "FRACTION OF STUDENTS COMMUTING", cmd);

                ValueArg<double> fractionActivePeople("a", "fracActive", "Fraction of people active", false, 0.75,
                                                      "FRACTION OF PEOPLE ACTIVE", cmd);

                ValueArg<unsigned int> populationSize("p", "populationSize", "Population size", false, 6000000,
                                                      "POPULATION SIZE", cmd);

                ValueArg<std::string> subMunicipalitiesFile("x", "subMinicipalities", "subMinicipalitiesFile", false,
                                                            "submunicipalities.csv", "OUTPUT FILE", cmd);

                cmd.parse(argc, static_cast<const char* const*>(argv));

                // --------------------------------------------------------------
                // Create logger.
                // --------------------------------------------------------------
                std::shared_ptr<spdlog::logger> logger =
                    stride::util::LogUtils::CreateCliLogger("stride_logger", "stride_log.txt");
                logger->set_level(spdlog::level::from_str(logLevel.getValue()));
                logger->flush_on(spdlog::level::err);

                // --------------------------------------------------------------
                // Configure.
                // --------------------------------------------------------------
                GeoGridConfig geoGridConfig{};
                geoGridConfig.input.populationSize                       = populationSize.getValue();
                geoGridConfig.input.fraction_1826_years_WhichAreStudents = fraction1826Students.getValue();
                geoGridConfig.input.fraction_active_commutingPeople      = fractionActiveCommutingPeople.getValue();
                geoGridConfig.input.fraction_student_commutingPeople     = fractionStudentCommutingPeople.getValue();
                geoGridConfig.input.fraction_1865_years_active           = fractionActivePeople.getValue();

                stride::util::RNManager::Info info;
                stride::util::RNManager       rnManager(info);

                GenGeoPopController genGeoPopController(logger, geoGridConfig, rnManager, citiesFile.getValue(),
                                                        commutingFile.getValue(), houseHoldFile.getValue(),
                                                        subMunicipalitiesFile.getValue());

                // --------------------------------------------------------------
                // Read input files.
                // --------------------------------------------------------------
                genGeoPopController.ReadDataFiles();
                logger->info("GeoGridConfig:\n\n{}", geoGridConfig);

                // --------------------------------------------------------------
                // Generate Geo
                // --------------------------------------------------------------
                logger->info("Starting Gen-Geo");
                genGeoPopController.GenGeo();
                logger->info("ContactCenters generated: {}", geoGridConfig.generated.contactCenters);
                logger->info("ContactPools generated: {}", geoGridConfig.generated.contactPools);
                logger->info("Finished Gen-Geo");

                // --------------------------------------------------------------
                // Generate Pop
                // --------------------------------------------------------------
                logger->info("Starting Gen-Pop");
                genGeoPopController.GenPop();
                logger->info("Finished Gen-Pop");

                // --------------------------------------------------------------
                // Write to file.
                // --------------------------------------------------------------
                logger->info("Writing to file...");
                GeoGridWriterFactory           geoGridWriterFactory;
                std::shared_ptr<GeoGridWriter> geoGridWriter = geoGridWriterFactory.createWriter(outputFile.getValue());
                std::ofstream                  outputFileStream(outputFile.getValue());
                geoGridWriter->write(genGeoPopController.GetGeoGrid(), outputFileStream);
                outputFileStream.close();

                logger->info("Done writing to file...");
        } catch (std::exception& e) {
                exit_status = EXIT_FAILURE;
                std::cerr << "\nEXCEPION THROWN: " << e.what() << std::endl;
        } catch (...) {
                exit_status = EXIT_FAILURE;
                std::cerr << "\nEXCEPION THROWN: Unknown exception." << std::endl;
        }
        return exit_status;
}
