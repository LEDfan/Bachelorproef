#include "createGeogrid.h"
#include <gengeopop/K12School.h>

using namespace gengeopop;

std::shared_ptr<GeoGrid> CreateGeoGrid(int locCount, int locPop, int k12SchoolCount, int houseHoldCount,
                                       int personCount, GeoGridConfig& geoGridConfig)
{
        std::vector<unsigned int> populationSample = {
            17, 27, 65, 40, 29, 76, 27, 50, 28, 62, 50, 14, 30, 36, 12, 31, 25, 72, 62, 4,  40, 52, 55, 50, 62,
            1,  30, 23, 6,  71, 50, 65, 8,  26, 44, 76, 23, 22, 69, 22, 4,  22, 48, 12, 57, 42, 36, 45, 73, 13,
            18, 58, 37, 43, 70, 36, 11, 54, 26, 8,  7,  6,  76, 15, 5,  23, 34, 74, 17, 33, 23, 54, 43, 72, 46,
            9,  57, 53, 75, 45, 29, 58, 67, 36, 63, 29, 24, 7,  35, 53, 40, 56, 60, 62, 53, 68, 78, 73, 24, 49,
            62, 12, 17, 58, 19, 77, 34, 40, 11, 65, 75, 27, 3,  34, 75, 1,  3,  28, 6,  72, 50, 69, 40, 42, 36,
            33, 77, 71, 47, 60, 53, 22, 13, 43, 62, 35, 47, 3,  68, 15, 53, 75, 2,  62, 54, 1,  32, 71, 55, 58,
            42, 69, 29, 15, 64, 66, 43, 15, 5,  70, 41, 43, 18, 19, 16, 57, 62, 12, 75, 76, 6,  67, 61, 11, 15,
            22, 51, 67, 38, 74, 66, 70, 1,  16, 71, 15, 40, 38, 11, 48, 45, 59, 11, 32, 18, 40, 46, 65, 21, 59,
            22, 2,  75, 5,  15, 10, 21, 59, 11, 61, 11, 9,  76, 76, 4,  66, 74, 74, 20, 18, 38, 5,  53, 29, 76,
            21, 73, 54, 43, 45, 12, 71, 70, 72, 36, 28, 18, 77, 53, 37, 40, 51, 71, 45, 18, 51, 22, 9,  17, 48,
            76, 73, 9,  27, 5,  68, 25, 16, 29, 58, 78, 75, 40, 8,  37, 63, 63, 76, 55, 47, 18, 4,  21, 39, 45,
            42, 20, 41, 40, 37, 38, 30, 48, 9,  40, 23, 68, 77, 21, 50, 18, 27, 54, 1,  32, 67, 27, 14, 4,  78};

        auto geoGrid = std::make_shared<GeoGrid>();

        std::size_t populationSampleId = 0;

        int personId = 0;

        for (int locI = 0; locI < locCount; locI++) {
                auto loc = std::make_shared<Location>(locI, 1, locPop);

                for (int schI = 0; schI < k12SchoolCount; schI++) {
                        auto k12School =
                            std::make_shared<K12School>(std::stoi(std::to_string(locI) + std::to_string(schI)));
                        k12School->Fill(geoGrid);
                        loc->AddContactCenter(k12School);
                }

                for (int hI = 0; hI < houseHoldCount; hI++) {
                        auto household =
                            std::make_shared<Household>(std::stoi(std::to_string(locI) + std::to_string(hI)));

                        household->Fill(geoGrid);

                        auto contactPool = household->GetPools()[0];

                        for (int pI = 0; pI < personCount; pI++) {
                                stride::Person* person = geoGrid->CreatePerson(
                                    personId, populationSample[populationSampleId], household->GetId(), 0, 0, 0, 0);

                                contactPool->AddMember(person);
                                populationSampleId++;
                                if (populationSampleId == 300) {
                                        populationSampleId = 0;
                                }
                                personId++;
                        }
                        loc->AddContactCenter(household);
                }
                geoGrid->AddLocation(loc);
        }
        return geoGrid;
}
