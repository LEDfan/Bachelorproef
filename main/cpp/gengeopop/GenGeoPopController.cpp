#include "GenGeoPopController.h"
#include <gengeopop/generators/CommunityGenerator.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/generators/HighSchoolGenerator.h>
#include <gengeopop/generators/HouseholdGenerator.h>
#include <gengeopop/generators/SchoolGenerator.h>
#include <gengeopop/generators/WorkplaceGenerator.h>
#include <gengeopop/io/ReaderFactory.h>
#include <gengeopop/populators/GeoGridPopulator.h>
#include <gengeopop/populators/HighSchoolPopulator.h>
#include <gengeopop/populators/HouseholdPopulator.h>
#include <gengeopop/populators/PrimaryCommunityPopulator.h>
#include <gengeopop/populators/SchoolPopulator.h>
#include <gengeopop/populators/SecondaryCommunityPopulator.h>
#include <gengeopop/populators/WorkplacePopulator.h>

namespace gengeopop {

GenGeoPopController::GenGeoPopController(GeoGridConfig& geoGridConfig, stride::util::RNManager& rnManager,
                                         std::string citiesFileName, std::string commutingFileName,
                                         std::string householdFileName, std::string subMunicipalitiesFileName)
    : m_geoGridConfig(geoGridConfig), m_rnManager(rnManager), m_geoGrid(nullptr), m_population(nullptr),
      m_citiesReader(nullptr), m_commutesReader(nullptr), m_householdsReader(nullptr),
      m_subMunicipalitiesReader(nullptr), m_citiesFileName(std::move(citiesFileName)),
      m_commutingFileName(std::move(commutingFileName)), m_householdsFileName(std::move(householdFileName)),
      m_subMunicipalitiesFileName(std::move(subMunicipalitiesFileName))
{
}

void GenGeoPopController::ReadDataFiles()
{
        if (m_population) {
                m_geoGrid = std::make_shared<GeoGrid>(m_population);
        } else {
                m_geoGrid = std::make_shared<GeoGrid>();
        }

        ReaderFactory readerFactory;

#pragma omp parallel sections
        {
#pragma omp section
                {
                        m_citiesReader = readerFactory.CreateCitiesReader(m_citiesFileName);
                        m_citiesReader->FillGeoGrid(m_geoGrid);
                }

#pragma omp section
                {
                        m_commutesReader = readerFactory.CreateCommutesReader(m_commutingFileName);
                }

#pragma omp section
                {
                        m_householdsReader = readerFactory.CreateHouseholdReader(m_householdsFileName);
                }
        }

        m_subMunicipalitiesReader = readerFactory.CreateSubMunicipalitiesReader(m_subMunicipalitiesFileName);

        m_citiesReader->FillGeoGrid(m_geoGrid);
        m_commutesReader->FillGeoGrid(m_geoGrid);
        m_subMunicipalitiesReader->FillGeoGrid(m_geoGrid);

        m_geoGridConfig.Calculate(m_geoGrid, m_householdsReader);

        m_geoGrid->finalize();
}

void GenGeoPopController::GengGeo()
{
        GeoGridGenerator geoGridGenerator(m_geoGridConfig, m_geoGrid);
        geoGridGenerator.addPartialGenerator(std::make_shared<SchoolGenerator>(m_rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<HighSchoolGenerator>(m_rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<WorkplaceGenerator>(m_rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<CommunityGenerator>(m_rnManager));
        geoGridGenerator.addPartialGenerator(std::make_shared<HouseholdGenerator>(m_rnManager));
        geoGridGenerator.generateGeoGrid();
}

void GenGeoPopController::GenPop()
{
        GeoGridPopulator geoGridPopulator(m_geoGridConfig, m_geoGrid);
        geoGridPopulator.addPartialPopulator(std::make_shared<HouseholdPopulator>(m_rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<SchoolPopulator>(m_rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<HighSchoolPopulator>(m_rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<PrimaryCommunityPopulator>(m_rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<SecondaryCommunityPopulator>(m_rnManager));
        geoGridPopulator.addPartialPopulator(std::make_shared<WorkplacePopulator>(m_rnManager));
        geoGridPopulator.populateGeoGrid();
}

std::shared_ptr<GeoGrid> GenGeoPopController::GetGeoGrid() { return m_geoGrid; }

void GenGeoPopController::UsePopulation(std::shared_ptr<stride::Population> pop) { m_population = std::move(pop); }

} // namespace gengeopop
