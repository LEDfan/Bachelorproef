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

double totalCompulsoryPupils(const std::vector<std::shared_ptr<Household>>& households)
{
        unsigned int totalPupils = 0;
        unsigned int total       = 0;
        for (const std::shared_ptr<Household>& household : households) {
                for (const std::shared_ptr<ContactPool>& contactPool : household->GetPools()) {
                        for (const std::shared_ptr<stride::Person>& person : *contactPool) {
                                total++;
                                if (person->GetAge() < 18 && person->GetAge() >= 6) {
                                        totalPupils++;
                                }
                        }
                }
        }

        return static_cast<double>(totalPupils) / static_cast<double>(total);
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

                cmd.parse(argc, static_cast<const char* const*>(argv));

                ReaderFactory readerFactory;

                auto citiesReader     = readerFactory.CreateCitiesReader(std::string(citiesFile.getValue()));
                auto commutesReader   = readerFactory.CreateCommutesReader(std::string(commutingFile.getValue()));
                auto houseHoldsReader = readerFactory.CreateHouseholdReader(std::string(houseHoldFile.getValue()));

                std::ofstream outputFileStream(outputFile.getValue());

                auto geoGrid = std::make_shared<GeoGrid>();

                citiesReader->FillGeoGrid(geoGrid);
                commutesReader->FillGeoGrid(geoGrid);

                geoGrid->finalize();

                GeoGridConfig geoGridConfig;
                geoGridConfig.populationSize            = geoGrid->getTotalPopulation();
                geoGridConfig.fraction_compulsoryPupils = totalCompulsoryPupils(houseHoldsReader->GetHouseHolds());

                std::cout << "Starting generation. Population size: " << geoGridConfig.populationSize
                          << ", compulsory pupils: " << geoGridConfig.fraction_compulsoryPupils << std::endl;

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
