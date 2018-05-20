#pragma once

#include "GeoGridConfig.h"
#include <gengeopop/io/CommutesReader.h>
#include <gengeopop/io/SubMunicipalitiesReader.h>
#include <spdlog/logger.h>
#include <util/RNManager.h>

namespace gengeopop {

/**
 * Controls the complete generation and population of a GeoGrid.
 */
class GenGeoPopController
{
public:
        GenGeoPopController(std::shared_ptr<spdlog::logger> logger, GeoGridConfig& geoGridConfig,
                            stride::util::RNManager& rnManager, std::string citiesFileName,
                            std::string commutingFileName, std::string householdFileName,
                            std::string subMunicipalitiesFileName);

        /// Reads the data files
        void ReadDataFiles();

        /// Generate and store the Geo part of the GeoGrid
        void GenGeo();

        /// Populate and store the Pop part of the GeoGrid
        void GenPop();

        /// Get the generated GeoGrid
        std::shared_ptr<GeoGrid> GetGeoGrid();

        /// Use the given Population storage during generation
        void UsePopulation(std::shared_ptr<stride::Population> pop, std::size_t regionId);

private:
        GeoGridConfig& m_geoGridConfig; ///< The GeoGridConfig used to generate

        stride::util::RNManager& m_rnManager; ///< The RNManager used

        std::shared_ptr<GeoGrid> m_geoGrid; ///< The generated GeoGrid

        std::shared_ptr<stride::Population> m_population; ///< The generated GeoGrid

        std::shared_ptr<CitiesReader>            m_citiesReader;            ///< The CitiesReader
        std::shared_ptr<CommutesReader>          m_commutesReader;          ///< The CommutesReader
        std::shared_ptr<HouseholdReader>         m_householdsReader;        ///< The HouseholdsReader
        std::shared_ptr<SubMunicipalitiesReader> m_subMunicipalitiesReader; ///< The SubMunicipalitiesReader
        std::shared_ptr<spdlog::logger>          m_logger;                  ///< The logger used

        std::string m_citiesFileName;     ///< Filename of the file which stores information about the cities
        std::string m_commutingFileName;  ///< Filename of the file which stores information about the commutes
        std::string m_householdsFileName; ///< Filename of the file which stores information about the households
        std::string
                    m_subMunicipalitiesFileName; ///< Filename of the file which stores information about the submunicipalities
        std::size_t m_regionId;                  ///< RegionId, used to create persons
};

} // namespace gengeopop
